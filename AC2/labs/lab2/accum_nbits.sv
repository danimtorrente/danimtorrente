module accum_nbits #(parameter WIDTH = 32) (
	input logic clk_i, // clock del registro
	input logic nrst_i, // boton de reset?
	input logic [WIDTH-1:0] x_i, // input 
	output logic [WIDTH-1:0] y_o // y_o de i se guarda en el registro, t_o de i+1 se suma a x_i
);

	logic [WIDTH-1:0] aux;
	
	adder_nbits #(.WIDTH(WIDTH)) almendras3( 
		.x_i(x_i),
		.y_i(aux), // registro, inicialmente 0 
		.c_i(1'b0), // NO SE, POR AHORA LO GUARDO O ALGO
		.s_o(y_o), // AL REGISTRO
		.c_o() // IGUAL NO SE
	);

	always_ff@(posedge clk_i, nrst_i) begin //Revisar si lo queremos en el flanco ascendente o descendente
		if (~nrst_i) begin // si nrst_i es 0 (activo), resetea el registro a 0
			aux <= 0;
		end else begin
			aux <= y_o;
		end
	end

endmodule






