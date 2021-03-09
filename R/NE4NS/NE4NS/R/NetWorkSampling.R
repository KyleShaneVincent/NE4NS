#' Network sampling
#' @description description
#' @param parameters listed and description
#' @return value that is being returned
#' @export
#' @useDynLib NetworkSampling.dll
#' @seealso see also
#' @examples
#' Network_Sample_Estimates()

Network_Sample_Estimates_R <- function()
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

Network_Sample_Estimates_Console_Output_R <- function() {
   result <- .C("NetworkSampleEstimatesConsoleOutput", tstep = as.integer(1000), paad0 = as.double(0.0167), naad0 = as.integer(1), paadtrace = as.double(0.5),
   aadreseeddesign = as.integer(1), paadrandom = as.double(0.01), naadtarget = as.integer(400), coupons = as.integer(0),
   nodeinputfilepath = as.character("c:\\data\\nodes.txt"),
   nodefiinputfilepath = as.character("c:\\data\\edges.txt"), edgeinputfilepath = as.character("c:\\data\\nodesfi.txt"), buffer = as.character(""))

   writeLines(result$buffer)
}