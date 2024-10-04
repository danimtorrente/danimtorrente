MODULE=adder_4bits
MODULE_N=adder_nbits
MODULE_ACCUM=accum_nbits

.PHONY:sim
sim: waveform.vcd

.PHONY:sim_n
sim_n: waveform_n.vcd

.PHONY:sim_accum
sim_accum: waveform_accum.vcd

.PHONY:build
build: simulator

.PHONY:build_n
build_n: simulator_nbits

.PHONY:build_accum
build_accum: simulator_accum

waveform.vcd: ./simulator
	@echo
	@echo "### SIMULATING ###"
	./simulator 

waveform_n.vcd: ./simulator2
	@echo
	@echo "### SIMULATING ###"
	./simulator2 

waveform_accum.vcd: ./simulator3
	@echo
	@echo "### SIMULATING ###"
	./simulator3

simulator: $(MODULE).sv tb_$(MODULE).sv
	@echo
	@echo "### BUILDING SIM ###"
	verilator --timing --timescale-override 1ps/1ps --trace --assert --x-assign unique --x-initial unique --binary $(MODULE).sv tb_$(MODULE).sv  -o ../simulator
	make -C obj_dir -f V$(MODULE).mk ../simulator

simulator_nbits: $(MODULE_N).sv tb_$(MODULE_N).sv
	@echo
	@echo "### BUILDING SIM N BITS###"
	verilator --timing --timescale-override 1ps/1ps --trace --assert --x-assign unique --x-initial unique --binary $(MODULE_N).sv tb_$(MODULE_N).sv  -o ../simulator2
	make -C obj_dir -f V$(MODULE_N).mk ../simulator2

simulator_accum: $(MODULE_ACCUM).sv tb_$(MODULE_ACCUM).sv
	@echo
	@echo "### BUILDING SIM N BITS###"
	verilator --timing --timescale-override 1ps/1ps --trace --assert --x-assign unique --x-initial unique --binary $(MODULE_ACCUM).sv tb_$(MODULE_ACCUM).sv  -o ../simulator3
	make -C obj_dir -f V$(MODULE_ACCUM).mk ../simulator3

.PHONY:lint
lint: $(MODULE).sv  $(MODULE_N).sv
	verilator --lint-only $(MODULE).sv $(MODULE_N).sv $(MODULE_ACCUM).sv

.PHONY: clean
clean:
	rm -rf ./obj_dir
	rm -rf ./simulator
	rm -rf ./simulator2
	rm -rf ./simulator3
	rm -rf waveform.vcd

