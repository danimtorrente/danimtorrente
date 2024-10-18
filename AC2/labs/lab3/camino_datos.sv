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



endmodule
