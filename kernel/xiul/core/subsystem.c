#include "xiul/subsystem.h"

#define XIUL_NO_INDEX ((size_t)-1)

typedef struct {
    const xiul_subsystem_descriptor_t *descriptor;
    unsigned char started;
} xiul_subsystem_entry_t;

static xiul_subsystem_entry_t registry[XIUL_SUBSYSTEM_MAX];
static size_t registry_count;
static size_t start_order[XIUL_SUBSYSTEM_MAX];
static size_t started_count;
static int startup_attempted;

static int name_equal(const char *a, const char *b)
{
    size_t i = 0;
    if (a == 0 || b == 0) return 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        ++i;
    }
    return a[i] == b[i];
}

static size_t find_subsystem(const char *name)
{
    size_t i;
    for (i = 0; i < registry_count; ++i) {
        if (name_equal(registry[i].descriptor->name, name)) return i;
    }
    return XIUL_NO_INDEX;
}

size_t xiul_subsystem_count(void)
{
    return registry_count;
}

xiul_status_t xiul_subsystem_register(
    const xiul_subsystem_descriptor_t *descriptor)
{
    size_t i;
    if (startup_attempted || descriptor == 0 || descriptor->name == 0 ||
        descriptor->name[0] == '\0' || descriptor->initialize == 0 ||
        descriptor->abi_version != XIUL_ABI_VERSION ||
        descriptor->phase < XIUL_PHASE_EARLY ||
        descriptor->phase > XIUL_PHASE_LATE ||
        descriptor->dependency_count > XIUL_SUBSYSTEM_MAX ||
        (descriptor->dependency_count != 0 && descriptor->dependencies == 0)) {
        return XIUL_E_INVALID;
    }
    for (i = 0; descriptor->name[i] != '\0'; ++i) {
        if (i >= XIUL_SUBSYSTEM_NAME_MAX) return XIUL_E_INVALID;
    }
    if (find_subsystem(descriptor->name) != XIUL_NO_INDEX) {
        return XIUL_E_DUPLICATE;
    }
    if (registry_count == XIUL_SUBSYSTEM_MAX) return XIUL_E_CAPACITY;
    registry[registry_count].descriptor = descriptor;
    registry[registry_count].started = 0;
    ++registry_count;
    return XIUL_OK;
}

/* DFS marks: 0=unseen, 1=visiting, 2=complete. */
static xiul_status_t visit(size_t index, unsigned char *marks)
{
    const xiul_subsystem_descriptor_t *d = registry[index].descriptor;
    size_t i;
    marks[index] = 1;
    for (i = 0; i < d->dependency_count; ++i) {
        size_t dependency = find_subsystem(d->dependencies[i]);
        xiul_status_t result;
        if (dependency == XIUL_NO_INDEX) return XIUL_E_DEPENDENCY;
        if (registry[dependency].descriptor->phase > d->phase) {
            return XIUL_E_DEPENDENCY;
        }
        if (marks[dependency] == 1) return XIUL_E_CYCLE;
        if (marks[dependency] == 0) {
            result = visit(dependency, marks);
            if (result != XIUL_OK) return result;
        }
    }
    marks[index] = 2;
    return XIUL_OK;
}

xiul_status_t xiul_subsystem_validate(void)
{
    unsigned char marks[XIUL_SUBSYSTEM_MAX] = {0};
    size_t i, j;
    for (i = 0; i < registry_count; ++i) {
        const xiul_subsystem_descriptor_t *d = registry[i].descriptor;
        for (j = 0; j < d->dependency_count; ++j) {
            size_t k;
            if (d->dependencies[j] == 0 || d->dependencies[j][0] == '\0') {
                return XIUL_E_INVALID;
            }
            for (k = j + 1; k < d->dependency_count; ++k) {
                if (name_equal(d->dependencies[j], d->dependencies[k])) {
                    return XIUL_E_DUPLICATE;
                }
            }
        }
        if (marks[i] == 0) {
            xiul_status_t result = visit(i, marks);
            if (result != XIUL_OK) return result;
        }
    }
    return XIUL_OK;
}

static xiul_status_t build_start_order(void)
{
    unsigned char emitted[XIUL_SUBSYSTEM_MAX] = {0};
    size_t output = 0;
    while (output < registry_count) {
        size_t i, best = XIUL_NO_INDEX;
        for (i = 0; i < registry_count; ++i) {
            const xiul_subsystem_descriptor_t *d;
            size_t j;
            int ready = 1;
            if (emitted[i]) continue;
            d = registry[i].descriptor;
            for (j = 0; j < d->dependency_count; ++j) {
                size_t dep = find_subsystem(d->dependencies[j]);
                size_t k;
                int dep_emitted = 0;
                for (k = 0; k < registry_count; ++k) {
                    if (emitted[k] && k == dep) dep_emitted = 1;
                }
                if (!dep_emitted) { ready = 0; break; }
            }
            if (!ready) continue;
            if (best == XIUL_NO_INDEX ||
                d->phase < registry[best].descriptor->phase) best = i;
        }
        if (best == XIUL_NO_INDEX) return XIUL_E_CYCLE;
        emitted[best] = 1;
        start_order[output++] = best;
    }
    return XIUL_OK;
}

void xiul_subsystem_stop_all(xiul_kernel_context_t *context)
{
    while (started_count != 0) {
        size_t index = start_order[--started_count];
        const xiul_subsystem_descriptor_t *d = registry[index].descriptor;
        if (registry[index].started && d->shutdown != 0) {
            d->shutdown(context);
        }
        registry[index].started = 0;
    }
}

xiul_status_t xiul_subsystem_start_all(xiul_kernel_context_t *context)
{
    size_t i;
    xiul_status_t result;
    if (startup_attempted) return XIUL_E_STATE;
    startup_attempted = 1;
    result = xiul_subsystem_validate();
    if (result != XIUL_OK) return result;
    result = build_start_order();
    if (result != XIUL_OK) return result;

    for (i = 0; i < registry_count; ++i) {
        size_t index = start_order[i];
        const xiul_subsystem_descriptor_t *d = registry[index].descriptor;
        result = d->initialize(context);
        if (result != XIUL_OK) {
            xiul_subsystem_stop_all(context);
            return XIUL_E_INIT;
        }
        registry[index].started = 1;
        ++started_count;
    }
    return XIUL_OK;
}

#ifdef XIUL_TESTING
void xiul_subsystem_test_reset(void)
{
    size_t i;
    for (i = 0; i < XIUL_SUBSYSTEM_MAX; ++i) {
        registry[i].descriptor = 0;
        registry[i].started = 0;
        start_order[i] = 0;
    }
    registry_count = 0;
    started_count = 0;
    startup_attempted = 0;
}
#endif
