CXX = g++
CXXFLAGS = -std=c++17
COMPILE = $(CXX) $(CXXFLAGS)

CPPGD_DPD = atom_class.o gdata_class.o molecule_class.o sup_class.o dictionary.o \
            gdata.o graph.o sup_functions.o \
			tui.o tui_main.o
cppgd: build_dir $(CPPGD_DPD)
	cd $(BUILD_FOLDER) && \
	$(COMPILE) $(CPPGD_DPD) -o cppgd

# cppgd_source folder
atom_class.o: ./cppgd_source/classes/atom_class.cpp
	$(COMPILE) -c ./cppgd_source/classes/atom_class.cpp -o $(BUILD_FOLDER)/atom_class.o

gdata_class.o: ./cppgd_source/classes/gdata_class.cpp
	$(COMPILE) -c ./cppgd_source/classes/gdata_class.cpp -o $(BUILD_FOLDER)/gdata_class.o

molecule_class.o: ./cppgd_source/classes/molecule_class.cpp
	$(COMPILE) -c ./cppgd_source/classes/molecule_class.cpp -o $(BUILD_FOLDER)/molecule_class.o

sup_class.o: ./cppgd_source/classes/sup_class.cpp
	$(COMPILE) -c ./cppgd_source/classes/sup_class.cpp -o $(BUILD_FOLDER)/sup_class.o

dictionary.o: ./cppgd_source/dictionary.cpp
	$(COMPILE) -c ./cppgd_source/dictionary.cpp -o $(BUILD_FOLDER)/dictionary.o

gdata.o: ./cppgd_source/gdata.cpp
	$(COMPILE) -c ./cppgd_source/gdata.cpp -o $(BUILD_FOLDER)/gdata.o

tui.o: ./cppgd_source/tui.cpp
	$(COMPILE) -c ./cppgd_source/tui.cpp -o $(BUILD_FOLDER)/tui.o

tui_main.o: ./cppgd_source/tui_main.cpp
	$(COMPILE) -c ./cppgd_source/tui_main.cpp -o $(BUILD_FOLDER)/tui_main.o

# sup_source folder
graph.o: ./sup_source/graph.cpp
	$(COMPILE) -c ./sup_source/graph.cpp -o $(BUILD_FOLDER)/graph.o

sup_functions.o: ./sup_source/sup_functions.cpp
	$(COMPILE) -c ./sup_source/sup_functions.cpp -o $(BUILD_FOLDER)/sup_functions.o


# others
build_dir:
	-mkdir $(BUILD_FOLDER)

clean:
	rm -rf $(BUILD_FOLDER)
