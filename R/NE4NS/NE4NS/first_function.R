#' Sums two values
#'
#' @param x real
#' @param y real
#'
#' @return real
#' @export
#' @examples
#' first_function(2,3)

dyn.load("C:\\Users\\SeraphimSpawn\\source\\repos\\NE4NS\\NE4NS\\C\\NetworkSampling\\x64\\Debug\\NetworkSampling.dll")
#dyn.load("C:\\Users\\SeraphimSpawn\\source\\repos\\NE4NS\\NE4NS\\C\\NetworkSampling\\Debug\\NetworkSampling.dll")
first_function <- function(x, y) {
  z = x + y
  return(z)
}

.C("scratch20")