MODULE=full_adder

.PHONY:sim
sim: waveform.vcd

.PHONY:build
build: simulator

.PHONY:view
view: waveform.vcd
	@echo
	@echo "### WAVES ###"
	gtkwave waveform.vcd -a gtkwave_setup.gtkw

waveform.vcd: ./simulator
	@echo
	@echo "### SIMULATING ###"
	./simulator 

simulator: $(MODULE).sv tb_$(MODULE).sv
	@echo
	@echo "### BUILDING SIM ###"
	verilator --trace --assert --x-assign unique --x-initial unique --binary $(MODULE).sv tb_$(MODULE).sv  -o ../simulator
	make -C obj_dir -f V$(MODULE).mk ../simulator

.PHONY:lint
lint: $(MODULE).sv
	verilator --lint-only $(MODULE).sv

view_cpp: waveform_cpp.vcd
	@echo
	@echo "### WAVES ###"
	gtkwave waveform_cpp.vcd -a gtkwave_setup.gtkw

waveform_cpp.vcd: ./simulator_cpp
	@echo
	@echo "### SIMULATING ###"
	./simulator_cpp

simulator_cpp: $(MODULE).sv tb_$(MODULE).cpp
	@echo
	@echo "### BUILDING SIM ###"
	verilator -Wall --trace --x-assign unique --x-initial unique -cc $(MODULE).sv --exe tb_$(MODULE).cpp -o ../simulator_cpp
	make -C obj_dir -f V$(MODULE).mk ../simulator_cpp

.PHONY: clean
clean:
	rm -rf ./obj_dir
	rm -rf ./simulator
	rm -rf ./simulator_cpp
	rm -rf waveform.vcd
	rm -rf waveform_cpp.vcd
