module full_adder (
    input logic x_i,
    input logic y_i,
    input logic c_i,

    output logic s_o,
    output logic c_o
);
    // Internal variables
    logic a_int;
    logic b_int;
    logic d_int;

    // Compute internal variables
    assign #100 a_int = x_i ^ y_i;
    assign #50 b_int = c_i & a_int;
    assign #50 d_int = x_i & y_i;

    // Compute the result
    assign #100 s_o = a_int ^ c_i;
    assign #100 c_o = b_int | d_int;

endmodule;
