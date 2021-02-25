.onLoad <- function(libname, pkgname) {
    library.dynam("NetworkSampling", pkgname, libname, now = TRUE)
    invisible()
}

.onUnLoad <- function(libpath) {
    library.dynam.unload("NetworkSampling", libpath)
    invisible()
}
