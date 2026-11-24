# XIUL integration source audit.
# This target verifies layout only; it does not claim to build or boot XIUL.

XIUL_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
REQUIRED_DIRS := mach linux freebsd core include/xiul io compat/linux bsd/net bsd/vfs bsd/posix bsd/nke

.PHONY: source-audit
source-audit:
	@set -eu; \
	for path in $(REQUIRED_DIRS); do \
		if [ ! -d "$(XIUL_ROOT)$$path" ]; then \
			echo "XIUL source-audit: missing $$path"; \
			exit 1; \
		fi; \
	done
	@test -f "$(XIUL_ROOT)xiul.toml"
	@test -f "$(XIUL_ROOT)README.md"
	@test -f "$(XIUL_ROOT)ARCHITECTURE.md"
	@echo "XIUL source-audit: source roots and integration layout present."
	@echo "This is a layout check, not a kernel build or boot test."
