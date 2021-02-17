#This script is used to build an R package; see here for details https://r-pkgs.org/index.html
#The package is titled "NE4NS" as it refers to New Estimates for Network Sampling
#The package provides end user, easy to use functions to implement an inference methodology developed by Steve Thompson
#See here for more details: https://arxiv.org/pdf/2002.01350.pdf




##Load the required packages
library(devtools)
library(roxygen2)
library(testthat)
library(knitr)

devtools::session_info()

library(devtools)

packageVersion("devtools")

library(tidyverse)
library(fs)


setwd("C:/Users/kyles/Desktop/NE4NS")
document()

#Generate the manual
devtools::build_manual(pkg=".", "C:/Users/kyles/Desktop/NE4NS")


##Create the package
create_package("C:/Users/kyles/Desktop/NE4NS")




##Start committing to git
use_git()





##Write a function and provide some explanation in the editor

first_function <- function(x, y) {
  z = x + y
}

use_r("first_function")




##Load and test

load_all()
rm(list = c("first_function"))
load_all()

first_function(x=10, y=15)

exists("first_function", where = globalenv(), inherits = FALSE)




##Check to see if it all looks good
check()






##Append a license
use_mit_license("Kyle Vincent")





##Start up some documentation
document()

?first_function




##Check it again and install

check()

install()





##Now close the console and reload to check that it works
library(NE4NS)

first_function(x=3, y=5)





##Test the function
use_testthat()

use_test("first_function")

test()






##We may want to also use other packages, and this would be the place to put it
#use_package("packagename")
#use_r("packagefcn")
#load_all()
#packagefcn(parameters)
#document()



##For github
use_readme_rmd()

build_readme()

check()

install()







#Save the example data sets in the data file
nodes = read.table("nodes.txt", header=TRUE)
usethis::use_data(nodes)

edges = read.table("edges.txt", header=TRUE)
usethis::use_data(edges)





