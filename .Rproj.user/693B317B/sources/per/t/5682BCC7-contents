#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates()

Network_Sample_Estimates_Custom_File_R <- function(output_file_path, node_input_file_path, node_fi_input_file_path, edge_input_file_path)
{
   result <- .C("NetworkSampleEstimates", tstep = as.integer(1000), paad0 = as.double(0.0167), naad0 = as.integer(1), paadtrace = as.double(0.5),
                aadreseeddesign = as.integer(1), paadrandom = as.double(0.01), naadtarget = as.integer(400), coupons = as.integer(0),
                outputfilepath = as.character(output_file_path), nodeinputfilepath = as.character(node_input_file_path),
                nodefiinputfilepath = as.character(node_fi_input_file_path), edgeinputfilepath = as.character(edge_input_file_path))
}

#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates()

Network_Sample_Estimates_Custom_Values_R <- function(t_step, paad_0, naad_0, paad_trace, aadres_eed_design, paad_random, naad_target, coupon_s, output_file_path)
{
   result <- .C("NetworkSampleEstimates", tstep = as.integer(t_step), paad0 = as.double(paad_0), naad0 = as.integer(naad_0), paadtrace = as.double(paad_trace),
                aadreseeddesign = as.integer(aadres_eed_design), paadrandom = as.double(paad_random), naadtarget = as.integer(naad_target), coupons = as.integer(coupon_s),
                outputfilepath = as.character("c:\\data\\output.txt"), nodeinputfilepath = as.character("c:\\data\\nodes.txt"),
                nodefiinputfilepath = as.character("c:\\data\\edges.txt"), edgeinputfilepath = as.character("c:\\data\\nodesfi.txt"))
}

#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates()
Network_Sample_Estimates_Custom_All_R <- function(t_step, paad_0, naad_0, paad_trace, aadres_eed_design, paad_random, naad_target, coupon_s, output_file_path, node_input_file_path, node_fi_input_file_path, edge_input_file_path)
{
   result <- .C("NetworkSampleEstimates", tstep = as.integer(t_step), paad0 = as.double(paad_0), naad0 = as.integer(naad_0), paadtrace = as.double(paad_trace),
                aadreseeddesign = as.integer(aadres_eed_design), paadrandom = as.double(paad_random), naadtarget = as.integer(naad_target), coupons = as.integer(coupon_s),
                outputfilepath = as.character(output_file_path), nodeinputfilepath = as.character(node_input_file_path),
                nodefiinputfilepath = as.character(node_fi_input_file_path), edgeinputfilepath = as.character(edge_input_file_path))
}


#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates()
Network_Sample_Estimates_Default_R <- function()
{
   result <- .C("NetworkSampleEstimates", tstep = as.integer(1000), paad0 = as.double(0.0167), naad0 = as.integer(1), paadtrace = as.double(0.5),
                aadreseeddesign = as.integer(1), paadrandom = as.double(0.01), naadtarget = as.integer(400), coupons = as.integer(0),
                outputfilepath = as.character("c:\\data\\output.txt"), nodeinputfilepath = as.character("c:\\data\\nodes.txt"),
                nodefiinputfilepath = as.character("c:\\data\\edges.txt"), edgeinputfilepath = as.character("c:\\data\\nodesfi.txt"))
}

#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates_Console_Output()

Network_Sample_Estimates_Console_Output_Custom_File_R <- function(node_input_file_path, node_fi_input_file_path, edge_input_file_path) {
   result <- .C("NetworkSampleEstimatesConsoleOutput", tstep = as.integer(1000), paad0 = as.double(0.0167), naad0 = as.integer(1), paadtrace = as.double(0.5),
                aadreseeddesign = as.integer(1), paadrandom = as.double(0.01), naadtarget = as.integer(400), coupons = as.integer(0),
                nodeinputfilepath = as.character(node_input_file_path),
                nodefiinputfilepath = as.character(node_fi_input_file_path), edgeinputfilepath = as.character(edge_input_file_path), buffer = as.character(""))

   writeLines(result$buffer)
}

#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates_Console_Output()

Network_Sample_Estimates_Console_Output_Custom_Values_R <- function(t_step, paad_0, naad_0, paad_trace, aadres_eed_design, paad_random, naad_target, coupon_s, output_file_path) {
   result <- .C("NetworkSampleEstimatesConsoleOutput", tstep = as.integer(t_step), paad0 = as.double(paad_0), naad0 = as.integer(naad_0), paadtrace = as.double(paad_trace),
                aadreseeddesign = as.integer(aadres_eed_design), paadrandom = as.double(paad_random), naadtarget = as.integer(naad_target), coupons = as.integer(coupon_s),
                nodeinputfilepath = as.character("c:\\data\\nodes.txt"),
                nodefiinputfilepath = as.character("c:\\data\\edges.txt"), edgeinputfilepath = as.character("c:\\data\\nodesfi.txt"), buffer = as.character(""))

   writeLines(result$buffer)
}

#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates_Console_Output()

Network_Sample_Estimates_Console_Output_Default_R <- function(t_step, paad_0, naad_0, paad_trace, aadres_eed_design, paad_random, naad_target, coupon_s, output_file_path, node_input_file_path, node_fi_input_file_path, edge_input_file_path) {
   result <- .C("NetworkSampleEstimatesConsoleOutput", tstep = as.integer(t_step), paad0 = as.double(paad_0), naad0 = as.integer(naad_0), paadtrace = as.double(paad_trace),
                aadreseeddesign = as.integer(aadres_eed_design), paadrandom = as.double(paad_random), naadtarget = as.integer(naad_target), coupons = as.integer(coupon_s),
                nodeinputfilepath = as.character(node_input_file_path),
                nodefiinputfilepath = as.character(node_fi_input_file_path), edgeinputfilepath = as.character(edge_input_file_path), buffer = as.character(""))

   writeLines(result$buffer)
}
