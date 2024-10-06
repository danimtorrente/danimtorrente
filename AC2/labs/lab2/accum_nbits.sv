module accum_nbits #(parameter WIDTH = 32) (
	input logic clk_i, // clock del registro
	input logic [WIDTH-1:0] nrst_i, // boton de reset?
	input logic [WIDTH-1:0] x_i, // input 
	output logic [WIDTH-1:0] y_o // y_o de i se guarda en el registro, t_o de i+1 se suma a x_i
);

//	logic [WIDTH:0] adder;
	genvar i;

//	logic b_sum;
//	logic b_c;
//	assign adder[0] = c_i;
//	assign c_o = adder[WIDTH];

	
	
generate
	for (i = 0; i < 2; ++i) begin
		adder_nbits #(.WIDTH(WIDTH)) almendras3( 
			.x_i(x_i[i]),
			.y_i(), // registro, inicialmente 0 
			.c_i(adder[i]), // NO SE, POR AHORA LO GUARDO O ALGO
			.s_o(s_o[i]), // AL REGISTRO
			.c_o(adder[i+1]) // IGUAL NO SE
		);
	end
endgenerate
endmodule
