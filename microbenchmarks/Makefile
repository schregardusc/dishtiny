TARGET_NAMES += dish2

TO_ROOT := $(shell git rev-parse --show-cdup)

include $(TO_ROOT)/microbenchmarks/MaketemplateForwarding

bench:
	./script/fixup_and_validate.sh
	python3 script/consolidate.py $$(find dish2 -name '*.json')
	for f in *run_type=iteration*.csv; do \
		python3 script/graph.py $$f; \
	done
	./script/upload.sh
