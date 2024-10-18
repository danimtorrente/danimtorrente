MODULE=buffer_circular
MODULE2=camino_datos

.PHONY:build
build: simulator

.PHONY:camino
camino: simulator2

simulator: $(MODULE).sv tb_$(MODULE).sv
	@echo
	@echo "### BUILDING SIM ###"
	verilator --timing --timescale-override 1ps/1ps --trace --assert --x-assign unique --x-initial unique --binary $(MODULE).sv tb_$(MODULE).sv  -o ../simulator
	make -C obj_dir -f V$(MODULE).mk ../simulator

simulator2: $(MODULE2).sv banco_registros.sv sumador.sv controlador.sv tb_$(MODULE2).sv
	@echo
	@echo "### BUILDING SIM ###"
	verilator --timing --timescale-override 1ps/1ps --trace --assert --x-assign unique --x-initial unique --binary banco_registros.sv sumador.sv controlador.sv $(MODULE2).sv tb_$(MODULE2).sv --top-module tb_$(MODULE2)  -o ../simulator2
	make -C obj_dir -f Vtb_$(MODULE2).mk ../simulator2

.PHONY: clean
clean:
	rm -rf ./obj_dir
	rm -rf ./simulator
	rm -rf ./simulator2
	rm -rf waveform.vcd
