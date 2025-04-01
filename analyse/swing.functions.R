## Helper functions for swing.R 
## Substantial material taken from code associated with Katz, Jonathan N., Gary King, and Elizabeth Rosenblatt. 
## "Theoretical foundations and empirical evaluations of partisan fairness in district-based democracies." 
## American Political Science Review 114.1 (2020): 164-178.

## It assumes the dataframe, dta, has the following columns:
## stabb: state abbreviation
## year
## sen: senate race or not
## dvotelag:  Lag of Democratic Vote total in the district
## dvote:  Democratic Vote total in the district
## votes0: Lag of Democratic Vote total in the district plus same for Republican
## votes1: Democratic Vote total in the district plus same for Republican
##

## Outputs as a tibble:
## l:   Number of legislative districts in the election
## s1:  True number of seats won in the election
## s1h: Estimated number of seats won in election 1 given v0 and swing

## various swing models

sw <- function(v0,vs0,d,str) {
  if(str=="ups") {
    di <- d    
  }
  else if(str=="prop") {
    di <- d*v0/vs0
  }
  else if(str=="pw") {
    ifelse (d>0, di <- d*(1-v0)/(1-vs0), di <- d*v0/vs0)
  }
  return (di) #  Estimate of district level swing
}

# estimate number of Democratic seats using input and various swing models
# district-level results
estimate <- function(dta,str){
  temp<- dta %>% 
  group_by(stabb,year,sen) %>% 
  mutate(vs0 = sum(dvotelag)/sum(dvotelag+rvotelag), vs1 = sum(dvote)/sum(dvote+rvote), s = vs1-vs0, 
  v0 = dvotelag/(dvotelag+rvotelag), v1 = dvote/(dvote+rvote), si = v1-v0, v1h = v0 + sw(v0,vs0,s,str) )
 
  dta <- tibble(stabb=temp$stabb,year=temp$year,sen=temp$sen,dno=temp$dno,s = temp$s, 
                si = temp$v1-temp$v0, v1h = temp$v1h, v0 = temp$v0, v1 = temp$v1, vs0 = temp$vs0, vs1 = temp$vs1
  )
  return(dta)
}

# state-level estimates based on district-level estimates
seats <- function(out2){# takes output of estimate as input
  temp <- out2 %>% group_by(stabb,year,sen) %>%summarize(vs0 = mean(vs0), vs1 = mean(vs1),seatp = mean(v1h>=0.5), seatr = mean(v1>=0.5), seatd = seatp - seatr, winner = mean((seatp >= 0.5 & seatr >=0.5) | (seatp <= 0.5 & seatr <=0.5)))
  return(temp)
}

# accuracy of district-level estimates
compute_accuracy <-function(out2){# takes output of estimate as input
    temp<- out2 %>% 
    group_by(stabb, year, dno) %>%
    mutate(error = v1 - v1h, 
           sign = as.numeric(as.logical( (sign(si) == sign(v1h-v0)) | (v1==v0) )), 
           winner = as.numeric(as.logical((v1 > 0.5 & v1h >0.5) | (v1<0.5 & v1h<0.5)) | (v1==0.5 & v1h==0.5)), 
           bounds = 1 - as.numeric(as.logical((v1h<0 | v1h>1)))
    )
  return(temp)
}

stats_accuracy <-function(temp) {# takes output of compute_accuracy as input
    out <- tibble(matrix(ncol = 7, nrow = 0))
    #v = tibble(str, df)
    #df = eval(as.name(df))
    w <- cbind(mean(temp$sign), mean(temp$winner),mean(temp$bounds),mean(temp$error^2), max(abs(temp$error)), 
               cor(temp$v1, temp$v1h, method = "pearson"), nrow(temp))
    out <-rbind(out, w) 
    colnames(out)<-c("sign","winner","bounds","msq","mabs","cor","nrow")
    return(out)  
  }
  
# accuracy of state-level estimates
stats_accuracy_seats <- function(out2){# takes output of seats as input
  temp<- tibble(matrix(ncol = 4, nrow = 0))
  win = mean(out2$winner)
  sfdmax = max(out2$seatd)
  sfdmin = min(out2$seatd)
  sfd2 = sqrt(mean(out2$seatd^2))
  temp<-rbind(temp,cbind(win,sfdmax,sfdmin,sfd2))
  return(temp)  
}


### produce regressions
create_regression<-function(dta) {
 
dta<-dta %>% mutate(decade = year%/%10)
  
out2<- tibble(matrix(ncol = 16, nrow = 0))
states <- unique(dta$stabb)
years <- unique(dta$year)
sencode <-unique(dta$sen)

  for (st in states) {
   for (y in years) {
     for (s in sencode) {
       temp<- dta %>%filter(stabb==st & year==y & sen == s)
       if(nrow(temp)>20) { # some weird cases with 1 district
         est<- estimate(temp,"ups")
         mod<- lm(est$v1 ~ est$v0)
         swing<-est$s[1] # all districts have same value from est so just pick first
         vs0<-est$vs0[1]
         vs1<-est$vs1[1]
         #print(swing)
         #print(mod$coefficients)
          row = c(c(st,y,y%/%10,s),as.numeric(summary(mod)$coefficients), summary(mod)$adj.r.squared, swing,vs0, vs1) 
         out2<-rbind(out2,row)
         #out2<-cbind(out2,est$vs0,est$vs1)
         out2<-na.omit(out2) # not working
        }
      }
    }
  }
colnames(out2)<-c("stabb","year","decade","sen","intercept","slope","isig","ssig","it","st","ip","sp","R2","sw","vs0","vs1")

return(out2)
}

goods <-function(out2, str) {

  s <- as.numeric(out2$slope)
  i <- as.numeric(out2$intercept)
  ss <- as.numeric(out2$ssig)
  is <- as.numeric(out2$isig)
  sw <- as.numeric(out2$sw)
  vs0 <- as.numeric(out2$vs0)
  vs1 <- as.numeric(out2$vs1)
  
l = s - 2*ss
r = s + 2*ss

dum <- 0 # quick fix to deal with ifelse

ifelse(str == "ups", slope <- 1, dum<-0)
ifelse(str == "prop", slope <- vs1/vs0, dum<-0)
ifelse (str =="pw", slope <- ifelse(vs1-vs0 > 0, (1-vs1)/(1-vs0), vs1/vs0),dum<-0)

  return(as.numeric(as.logical(l <= slope & slope <= r)))

}

goodi <-function(out2, str) {
  
  s <- as.numeric(out2$slope)
  i <- as.numeric(out2$intercept)
  ss <- as.numeric(out2$ssig)
  is <- as.numeric(out2$isig)
  sw <- as.numeric(out2$sw)
  vs0 <- as.numeric(out2$vs0)
  vs1 <- as.numeric(out2$vs1)
  
  l = i - 2*is
  r = i + 2*is
  
  dum <- 0 # quick fix to deal with ifelse 
  ifelse(str == "ups", int <- vs1-vs0,dum<-0)
  ifelse(str == "prop", int <- 0,dum<-0)
  ifelse(str =="pw", int <- ifelse(vs1-vs0>0, (vs1-vs0)/(1-vs0), 0),dum<-0)

  return(as.numeric(as.logical(l <= int & int <= r)))
}

stats_regression<- function(temp3) { # takes output from analyse_regression() 

return (temp3 %>% group_by(str, decade) %>% summarize(ms = mean(as.numeric(slope)), mi = mean(as.numeric(intercept)), mgs = mean(goods), mgi = mean(goodi)))

}

  
