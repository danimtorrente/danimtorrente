module CaminoDatos #(
  parameter WIDTH = 64,
  parameter NUMREG = 8,
  localparam INDEX_SIZE = $clog2(NUMREG)
)(
  input logic 					clk_i, 			// Senyal de reloj
  input logic 					rstn_i, 		// Senyal negada de reset
  input logic 					inicio_i,       // Senyal de inicio
  output logic  				fin_o,			// Senyal de finalizacion
  output logic [WIDTH-1:0]  	data_o,			// Senyal de debug de datos
  output logic [INDEX_SIZE-1:0]	regId_o         // Senyal de debug de id registro escrito
);
////////////////////////////////////////////////////////////////////////////////////////////////////

	logic [WIDTH-1:0] sum_write;
	logic [INDEX_SIZE-1:0] id_write;
	logic write_enable;
	logic [INDEX_SIZE-1:0] read_a;
	logic [INDEX_SIZE-1:0] read_b;
	logic [WIDTH-1:0] alu_a;
	logic [WIDTH-1:0] alu_b;
	
sumador #(.WIDTH(WIDTH)) sumr (                                                             // Sumador
        .x_i(alu_a),
        .y_i(alu_b),
        .s_o(sum_write)
);

controlador #(.WIDTH(WIDTH), .NUMREG(NUMREG), .NUMSUMA(12)) ctrl (                             // Controlador
        .clk_i(clk_i),
        .rstn_i(rstn_i),
        .inicio_i(inicio_i),
        .fin_o(fin_o),
        .id_o(id_write),
        .we_o(write_enable),
        .A_o(read_a),
        .B_o(read_b)
);

bancoRegistros #(.WIDTH(WIDTH), .NUM(NUMREG)) br (                                          // Banco de registros
	.clk_i(clk_i),
	.rstn_i(rstn_i),
	.A_i(read_a),
	.B_i(read_b),
	.we_i(write_enable),
	.dato_i(sum_write),
	.wid_i(id_write),
	.RegA_o(alu_a),
	.RegB_o(alu_b)
);

always_comb begin
	if (!fin_o) begin
		data_o = 0;
		regId_o = 0;
	end else begin
		data_o = sum_write;
		regId_o = id_write;
	end
end
endmodule
