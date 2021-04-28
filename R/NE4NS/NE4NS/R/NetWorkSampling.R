#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates_Console_Output()
#' 
Network_Sample_Estimates <- function(t_step = 1000, paad_0 = 0.0167, naad_0 = 1, paad_trace = 0.5, aadres_eed_design = 1, paad_random = 0.01, naad_target = 400, 
                                     coupon_s = 0,  output_file_path = as.character("c:\\data\\output.txt"), node_input_file_path = as.character("c:\\data\\nodes.txt"),
                                     node_fi_output_file_path = as.character("c:\\data\\nodesfi.txt"), edge_input_file_path = as.character("c:\\data\\edges.txt")) {
   
   result <- .C("NetworkSampleEstimatesConsoleOutput", tstep = as.integer(t_step), paad0 = as.double(paad_0), naad0 = as.integer(naad_0), paadtrace = as.double(paad_trace),
            aadreseeddesign = as.integer(aadres_eed_design), paadrandom = as.double(paad_random), naadtarget = as.integer(naad_target), coupons = as.integer(coupon_s),
            outputfilepath = as.character(output_file_path), nodeinputfilepath = as.character(node_input_file_path),
            nodefiinputfilepath = as.character(node_fi_output_file_path), edgeinputfilepath = as.character(edge_input_file_path), buffer = as.character(""))

   writeLines(result$buffer)
}
