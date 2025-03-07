EXE = result
EXETEST = testResult

OBJS_DIR = .objs

OBJS_EXE = main.o qtvar.o qtcount.o quadtree.o stats.o
OBJS_EXETEST = testComp.o qtvar.o qtcount.o quadtree.o stats.o catch_config.o
OBJS_PROVIDED = RGBAPixel.o lodepng.o PNG.o

CXX = clang++
CXXFLAGS = -std=c++14 -c -g -O0 -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-unused-variable
LD = clang++
LDFLAGS = -std=c++14 -lpthread -lm

# Rules
all: $(EXE) $(EXETEST)

# Pattern rules for object files
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Pattern rules for compression
$(OBJS_DIR)/%.o: compression/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJS_DIR)/%.o: compression/lodepng/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Create directories
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR) $(OUT_DIR)

# Rules for executables... we can use a pattern for the -asan versions, but, unfortunately, we can't use a pattern for the normal executables
$(EXE):
	$(LD) $^ $(LDFLAGS) -o $@
$(EXETEST):
	$(LD) $^ $(LDFLAGS) -o $@

# Executable dependencies
$(EXE): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS_EXE)) $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS_PROVIDED))
$(EXETEST): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS_EXETEST)) $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS_PROVIDED))

# Include automatically generated dependencies
-include $(OBJS_DIR)/*.d

clean:
	rm -rf $(EXE) $(EXETEST) $(OBJS_DIR)

.PHONY: all clean