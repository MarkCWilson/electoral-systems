## swing.R
## Mark C. Wilson <markwilson@umass.edu>
##
## Purpose: Empirical part of Grofman-Wilson paper "Models of inter-election change in partisan vote share"
## Journal of Theoretical Politics 34.4 (2022): 481-498. Found at https://doi.org/10.1177/09516298221123263
##
##

#####################################################
##### Load libraries and helper functions

library(tidyverse)
#library(haven)
#library(extrafont)
#library(fontcm)

#font_install('fontcm') # Needed to produce final graphs with CM fonts to match LaTeX
#loadfonts()

setwd("~/Dropbox/Mark_research/collective_decisions/electoral-engineering/GrWi2016")
source('swing.functions.R')

#####################################################
##### Output directory for figures

if (!dir.exists('figures')){
  dir.create('figures')
} else {

}

#####################################################
### Data Wrangling
###
### 
sl6816.orig.dta <- read.table('data/102slersuoacontest20181024.csv', header=TRUE,sep="\t")

#print("original data")
#print(as_tibble(sl6816.orig.dta))

sl6816.dta <- sl6816.orig.dta %>% ### still need to consider the uncontested elections
  rename(
    stabb = sab
  ) %>% 
  group_by(stabb, year, sen) %>% 
  filter(all(dtype == 1) & all(etype == "g")) %>% 
  mutate(
    unc = uncont,
    unc.lag = uncontlag
    ) %>% 
  filter(
    dontuse == 0  &
      bigthird == 0 &
      !is.na(dvote) &
      (dvotelag+rvotelag >0) &
      (dvote+rvote >0)
  ) %>%
  
  select(stabb, year, sen, dno, redist, dvote, dvotelag, rvote, rvotelag, unc, unc.lag)

#print("cleaned data")
#print(sl6816.dta)

## Create a lag safe version for prop analysis. Given inconsistent naming convention for districts plus redistricting,
## we can not safely calculate the lag vote share.

#print("lag safe data")
sl6816.lag.safe.dta <- sl6816.dta %>%
  group_by(stabb, year, sen) %>% 
  filter(sum(redist) == 0 & !is.na(dvotelag)  ) 

#print(sl6816.lag.safe.dta)

#####################################################
### data sets
###

#print("KKR20 but uncont still not done")
#KKR20.dta <- sl6816.lag.safe.dta %>%
#  group_by(stabb, year, sen) %>%
  # Drop state elections with too many (greater than a half) uncontested districts
#  filter(mean(unc) <= 0.50) %>%
#  filter(n() >= 20)  

#print("GW20 basic dataset")
GW20.dta <-  sl6816.lag.safe.dta %>%
  group_by(stabb, year, sen) %>% filter(sen==0)
#print(GW20.dta) 
 
#print("only contested elections")  
GW20.cont.dta <-  GW20.dta %>%
  # Drop all uncontested elections everywhere
  filter(unc == 0 & unc.lag == 0) 
#print(GW20.cont.dta)

GW20.unc2.dta <- GW20.dta %>%
  # for uncontested, impute 0.75 for winner and 0.25 for loser
mutate(rvote = ifelse(rvote==0 & unc==1,0.25*dvote,ifelse(unc==1,0.75*rvote,rvote)), 
       dvote = ifelse(dvote==0 & unc==1,0.25*rvote,ifelse(unc==1,0.75*dvote,dvote)))

#GW20.far.dta <-  GW20.dta %>%
#  filter(v0>0.7 | v0<0.3) 

GW20.close.dta <-  GW20.cont.dta %>%
  filter(dvote/(dvote+rvote)>0.475 & dvote/(dvote+rvote)<0.525) 

#GW20.close.unc2.dta <- GW20.unc2.dta %>%
#  filter(v1>0.475 & v1<0.525) 

#GW20.close.cont.dta <- GW20.close.dta

# functions to generate the results for various datasets and swing models

compare_models <-function(dtaa,stra) {
  
  out <- tibble(matrix(ncol = 9, nrow = 0))
  for(dta in dtaa) {
   for (str in stra) {
      df = paste("GW20",dta,".dta", sep="")
      df<- eval(as.name(df))
      out<-rbind(out,cbind(dta,str,stats_accuracy(compute_accuracy(estimate(df,str)))))
    }
}
  colnames(out)<-c("str","df","win","sgn","bounds","meansquare", "max","rho","nrow")
  return(out)
}
#ggplot(res, aes(res$winner, res$sign, colour = res$model)) + geom_point()

# now the regression analysis 

analyse_regression <- function(out2) {
  sp <- as.numeric(out2$sp)
  bad <- sum(is.na(sp))
  sp <- sp[!is.na(sp)]
  l <- length(sp)
  
  oo<- tibble(matrix(ncol = 17, nrow = 0))
    for(str in stra) {
      strr<- rep(str,nrow(out2))
     # print(strr)
      o<- cbind(out2,strr) 
      
     # print(o)
      goos <- as_tibble(goods(o,str))
      gooi <- as_tibble(goodi(o,str))
      o3<- cbind(o, goos,gooi)
      oo<-rbind(oo,o3)
    }

  
  colnames(oo)<- c(colnames(out2), "str", "goods", "goodi")
  return(oo)
}  

compare_regressions <- function(dtaa, stra) {
  
  out <- tibble(matrix(ncol = 7, nrow = 0))
  for(dta in dtaa) {
    for (str in stra) {
      df = paste("GW20",dta,".dta", sep="")
      df<- eval(asaname(df))
      temp<-stats_regression(analyse_regression(create_regression(df)))
      dtar<- rep(dta,nrow(temp))
      out<-rbind(out,cbind(dtar,temp))
    }
  }
 # colnames(out)[1]<-"data"
  return(out)
}

# now do it

stra <- c("ups","prop","pw")
dtaa <- c("",".unc2",".cont",".close")

temp1 <- compare_models(dtaa,stra)
temp2 <-compare_regressions(dtaa,stra)

#####################################################
### System Info
### For replication

sessionInfo()


