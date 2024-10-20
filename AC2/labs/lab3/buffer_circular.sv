module BufferCircular #(
  parameter WIDTH = 64,
  parameter NUM = 8,
  localparam INDEX_SIZE = $clog2(NUM)
)(
  input logic 				clk_i, 			// Senyal de reloj
  input logic 				rstn_i, 		// Senyal negada de reset
  input logic 				insercion_i, 	// Senyal de insercion
  input logic [WIDTH-1:0] 	dato_i, 		// Dato a insertar
  input logic 				delecion_i, 	// Senyal de delecion
  output logic [WIDTH-1:0] 	dato_o, 		// Dato leido
  output logic 				vacia_o, 		// Senyal de cola vacıa
  output logic 				llena_o 		// Senyal de cola llena
);

// Instanciamos los registros WIDTHxNUM
logic [WIDTH-1:0] registers_q [NUM-1:0];

// Instanciamos punteros de control
logic [INDEX_SIZE-1:0] cola_q, cabeza_q;

// Instanciamos senyales de control
logic permiso_insercion, permiso_delecion;
logic [INDEX_SIZE:0] num_q;

// Logica de insercion
assign permiso_insercion = (num_q < NUM) & (insercion_i);

// Logica de delecion
assign permiso_delecion = (num_q > 0) & (delecion_i);

// Logica de Lectura. Solo lee si hay un dato
assign dato_o = (vacia_o) ? 'h0 : registers_q[cabeza_q];

// Logica de Escritura.
always_ff @(posedge clk_i, negedge rstn_i) begin
  if (~rstn_i) begin
    for (int i=0; i<NUM; ++i) begin
      registers_q[i] <= 'h0;
    end
  end else if (permiso_insercion) begin
    registers_q[cola_q] <= dato_i;
  end
end

// Logica de Control
always_ff @(posedge clk_i, negedge rstn_i) begin
  if (!rstn_i) begin
    cabeza_q <= 'h0;
    cola_q <= 'h0;
    num_q <= 'h0;
  end else begin
    cabeza_q <= cabeza_q + {2'b0, permiso_delecion};
    cola_q <= cola_q + {2'b0, permiso_insercion};
    num_q <= num_q + {3'b0, permiso_insercion} - {3'b0, permiso_delecion};
  end
end

assign vacia_o = (num_q == 0);

assign llena_o = ((num_q == NUM) | !(rstn_i));

endmodule
