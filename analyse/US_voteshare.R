## Mark C. Wilson <wilson.mark.c@gmail.com>
##
## Purpose: Analysis of two-party Dem/Rep vote share (excludes votes for other parties) by district
## Assumes columns state,district,party,votes in input csv
##
##


library(reshape2)
dta = "/Users/mwilson/Dropbox/Mark_research/simulation/inputs/electoral/USA/2014_US_long.csv"
t <-read.csv(dta, header=TRUE, sep = ",")
t$VOTES<-as.numeric(as.character(t$VOTES))
t[is.na(t)] <- 0

t1 <- subset(t, PARTY == "R" | PARTY == "D")
t2 <- unique(t1)
#t2<-melt(t1, id = c("STATE", "DISTRICT"))
#head(t2)

tt <-dcast(t2, STATE + DISTRICT ~ PARTY, value.var="VOTES", fun.aggregate=sum)
#head(tt)

Dem_vote = tt[,3]/(tt[,3]+tt[,4])
Dem_vote<-as.data.frame(Dem_vote)

colnames(Dem_vote)<-c("votes")
p5<-ggplot(Dem_vote, aes(x = votes)) + geom_histogram(colour = "black", fill = "white", binwidth=0.01) + labs(title = "") + labs(x = "vote share in district", y = "frequency")+xlim(0,1)+ylim(0,20)
show(p5)

Rep_vote = tt[,4]/(tt[,3]+tt[,4])
Rep_vote<-as.data.frame(Rep_vote)

colnames(Rep_vote)<-c("votes")
p6<-ggplot(Rep_vote, aes(x = votes)) + geom_histogram(colour = "black", fill = "white", binwidth=0.015) + labs(title = "") + labs(x = "vote share in district", y = "frequency")+xlim(0,1)+ylim(0,20)
show(p6)


