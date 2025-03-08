CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -fprofile-arcs -ftest-coverage
LDFLAGS = -lgcov

SRC_DIR = src
INCLUDE_DIR = include
TESTS_DIR = tests
BUILD_DIR = build
REPORTS_DIR = reports

CLANG = clang++
CLANG_FLAGS = -std=c++11 -Wall -Wextra -g -fprofile-instr-generate -fcoverage-mapping
CLANG_BUILD_DIR = $(BUILD_DIR)/clang

SRCS = $(SRC_DIR)/levenshtein.cpp $(SRC_DIR)/complex_conditions.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

SC_TEST = $(TESTS_DIR)/sc_tests.cpp
DC_TEST = $(TESTS_DIR)/dc_tests.cpp
MCDC_TEST = $(TESTS_DIR)/mcdc_tests.cpp

SC_TARGET = $(BUILD_DIR)/sc_test
DC_TARGET = $(BUILD_DIR)/dc_test
MCDC_TARGET = $(BUILD_DIR)/mcdc_test

PROOF_SRC = $(TESTS_DIR)/coverage_proof.cpp
PROOF_TARGET = $(BUILD_DIR)/coverage_proof

COMPARISON_SCRIPT = compare_coverage.sh
COMPARISON_TARGET = $(BUILD_DIR)/coverage_comparison

DC_MC_PROOF_TARGET = $(BUILD_DIR)/dc_mc_proof
$(DC_MC_PROOF_TARGET): $(TESTS_DIR)/dc_mc_proof.cpp $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(PROOF_TARGET): $(PROOF_SRC) $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

proof: $(PROOF_TARGET)
	$(PROOF_TARGET)

$(COMPARISON_TARGET): $(TESTS_DIR)/coverage_comparison.cpp $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

comparison: $(COMPARISON_TARGET)
	$(COMPARISON_TARGET)

compare-all: all
	bash $(COMPARISON_SCRIPT)

.PHONY: all clean test sc dc mcdc report-sc report-dc report-mcdc

all: $(SC_TARGET) $(DC_TARGET) $(MCDC_TARGET)

$(BUILD_DIR) $(REPORTS_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SC_TARGET): $(SC_TEST) $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(DC_TARGET): $(DC_TEST) $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(MCDC_TARGET): $(MCDC_TEST) $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

sc: $(SC_TARGET) | $(REPORTS_DIR)
	rm -f $(BUILD_DIR)/*.gcda
	$(SC_TARGET)
	@echo "\n--- Отчет о Statement Coverage ---"
	gcovr -r . --txt --fail-under-line=0

dc: $(DC_TARGET) | $(REPORTS_DIR)
	rm -f $(BUILD_DIR)/*.gcda
	$(DC_TARGET)
	@echo "\n--- Отчет о Decision Coverage ---"
	gcovr -r . --txt --branches --fail-under-branch=0

mcdc: $(MCDC_TARGET) | $(REPORTS_DIR)
	rm -f $(BUILD_DIR)/*.gcda
	$(MCDC_TARGET)
	@echo "\n--- Отчет о MC/DC Coverage ---"
	gcovr -r . --txt --branches --fail-under-branch=0

report-sc: sc | $(REPORTS_DIR)
	gcovr -r . --html --html-details -o $(REPORTS_DIR)/sc_coverage.html

report-dc: dc | $(REPORTS_DIR)
	gcovr -r . --html --html-details -o $(REPORTS_DIR)/dc_coverage.html

report-mcdc: mcdc | $(REPORTS_DIR)
	mkdir -p $(REPORTS_DIR)/mcdc
	cd $(BUILD_DIR) && gcov -a -b -f -u levenshtein.o
	mv $(BUILD_DIR)/*.gcov $(REPORTS_DIR)/mcdc/ 2>/dev/null || true

test: sc dc mcdc

clean:
	rm -rf $(BUILD_DIR) $(REPORTS_DIR) *.gcov *.gcda *.gcno

dc-mc-proof: $(DC_MC_PROOF_TARGET)
	$(DC_MC_PROOF_TARGET)

$(CLANG_BUILD_DIR):
	mkdir -p $@

clang-build: | $(CLANG_BUILD_DIR)
	$(CLANG) $(CLANG_FLAGS) src/levenshtein.cpp src/complex_conditions.cpp tests/mcdc_tests.cpp -o $(CLANG_BUILD_DIR)/mcdc_test

clang-mcdc: clang-build
	rm -f default.profraw
	$(CLANG_BUILD_DIR)/mcdc_test
	llvm-profdata merge -sparse default.profraw -o mcdc.profdata
	@echo "\n--- Отчет о MC/DC Coverage с использованием Clang ---"
	llvm-cov report $(CLANG_BUILD_DIR)/mcdc_test -instr-profile=mcdc.profdata
	mkdir -p $(REPORTS_DIR)
	llvm-cov show $(CLANG_BUILD_DIR)/mcdc_test -instr-profile=mcdc.profdata src/levenshtein.cpp --format=html > $(REPORTS_DIR)/clang_mcdc_coverage.html

clang-dc-build: | $(CLANG_BUILD_DIR)
	$(CLANG) $(CLANG_FLAGS) src/levenshtein.cpp src/complex_conditions.cpp tests/dc_tests.cpp -o $(CLANG_BUILD_DIR)/dc_test

clang-dc: clang-dc-build
	rm -f default.profraw
	$(CLANG_BUILD_DIR)/dc_test
	llvm-profdata merge -sparse default.profraw -o dc.profdata
	@echo "\n--- Отчет о DC Coverage с использованием Clang ---"
	llvm-cov report $(CLANG_BUILD_DIR)/dc_test -instr-profile=dc.profdata
	mkdir -p $(REPORTS_DIR)
	llvm-cov show $(CLANG_BUILD_DIR)/dc_test -instr-profile=dc.profdata src/levenshtein.cpp --format=html > $(REPORTS_DIR)/clang_dc_coverage.html
