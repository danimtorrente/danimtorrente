module adder_nbits #(parameter WIDTH) (
	input logic [WIDTH-1:0] x_i,
	input logic [WIDTH-1:0] y_i,
	input logic c_i,
	output logic [WIDTH-1:0] s_o,
	output logic c_o
);

	logic [WIDTH:0] adder;
	genvar i;
	assign adder[0] = c_i;
	assign c_o = adder[WIDTH];
generate
	for (i = 0; i < WIDTH; ++i) begin
		full_adder almendras2(
			.x_i(x_i[i]),
			.y_i(y_i[i]), 
			.c_i(adder[i]), 
			.s_o(s_o[i]),
			.c_o(adder[i+1]) 
		);
	end
endgenerate
endmodule
