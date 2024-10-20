module controlador #(
  parameter WIDTH = 64,
  parameter NUMREG = 8,
  parameter NUMSUMA = 6,
  localparam INDEX_SIZE = $clog2(NUMREG)
)(
  input logic 					clk_i, 		// Senyal de reloj
  input logic 					rstn_i, 	// Senyal negada de reset
  input logic 					inicio_i, 	// Senyal de inicio
  output logic		 			fin_o, 		// Senyal de fin
  output logic [INDEX_SIZE-1:0] id_o, 		// Id de registro de escritura
  output logic 					we_o, 		// Senyal de permiso de escritura
  output logic [INDEX_SIZE-1:0] A_o, 		// Id de registro de lectura A
  output logic [INDEX_SIZE-1:0] B_o 		// Id de registro de lectura B
);

// Instanciamos el estado del automata
logic estado_q, proximo_estado_d;
// Instanciamos la senyal interna de finalizaci´on
logic next_fin;
// Logica de pr´oximo estado

always_comb begin
  proximo_estado_d = 1'b0; // Valor por defecto
  if (!rstn_i) begin // Transicion a 0 en reset
    proximo_estado_d = 1'b0;
  end else if ((estado_q == 1'b0) & (inicio_i)) begin // Condicion de inicio
    proximo_estado_d = 1'b1;
  end else if (next_fin) begin // Condicion de finalizacion
    proximo_estado_d = 1'b0;
  end else if (estado_q == 1'b1) begin // Estado de operacion
    proximo_estado_d = 1'b1;
  end // else toma el valor defecto 1’b0
end

// Actualizacion de automata principal
always_ff @(posedge clk_i, negedge rstn_i) begin
  if (!rstn_i) begin // Reset
    estado_q <= 'h0;
  end begin
    estado_q <= proximo_estado_d;
  end
end

// Instanciamos los contadores
logic [INDEX_SIZE-1:0] cnt_q, proximo_cnt_d; // contador de pasos
logic [INDEX_SIZE-1:0] idA_q, proximo_idA_d; // contador de id A
logic [INDEX_SIZE-1:0] idB_q, proximo_idB_d; // contador de id B
logic [INDEX_SIZE-1:0] id_q, proximo_id_d; // contador de id escritura

// Logica de proximo estado para los contadores
always_comb begin // ESTO ES LO QUE SE HA MODIFICADO
  if (!rstn_i) begin // Reset
    proximo_cnt_d = 'b0;
    proximo_idA_d = 4'h1;
    proximo_idB_d = 'b0;
    proximo_id_d = 'b0;
  end else if (cnt_q == 2*NUMSUMA) begin // Fin del proceso
    proximo_cnt_d = 'b0;
    proximo_idA_d = 4'h1;
    proximo_idB_d = 'b0;
    proximo_id_d = 'b0;
  end else begin // Operaciones normales
    proximo_cnt_d = cnt_q + 4'h1;
    proximo_id_d = 'b0;
    if (!proximo_estado_d) begin 
      proximo_idA_d = idA_q + 4'h1;
      proximo_idB_d = 'b0;
    end else begin
      proximo_idA_d = idA_q;
      proximo_idB_d = 'b0;
    end
  end
end


// Actualizacion de los automatas secundarios
always_ff @(posedge clk_i, negedge rstn_i) begin
  if (!rstn_i) begin // Reset
    cnt_q <= 'h0;
    idA_q <= 'h0;
    idB_q <= 'h0;
    id_q  <= 'h0;
  end else begin
    cnt_q <= proximo_cnt_d;
    idA_q <= proximo_idA_d;
    idB_q <= proximo_idB_d;
    id_q <= proximo_id_d;
  end
end

// Logica de finalizacion. (Se puede simplificar)
always_comb begin
  next_fin = 1'b0; // Valor por defecto
  if (!rstn_i) begin // Valor en reset
    next_fin = 1'b0;
  end else if (estado_q == 1'b0) begin // Valor en el estado 0
    next_fin = 1'b0;
  end else if ((estado_q == 1'b1) & (cnt_q == 'h0)) begin // Condicion de finalizacion
    next_fin = 1'b1;
  end // Cualquier otra combinacion
end

// Asignacion de condicion de fin
assign fin_o = next_fin;

// Asignacion de registro de escritura
assign id_o = id_q;

// Asignacion de permiso de escritura
assign we_o = estado_q;

// Asignacion de registros de lectura
assign A_o = idA_q;
assign B_o = idB_q;

endmodule
