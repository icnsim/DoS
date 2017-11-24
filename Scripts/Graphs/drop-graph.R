# drop-graph.R
# Gera gráficos de pacotes rejeitados pelos roteadores.

# install.packages ('ggplot2')
library(ggplot2)
## # install.packages ('scales')
## library (scales)
# install.packages('doBy')
library(doBy)

#########################
# Rate trace processing #
#########################
data = read.table("drop-trace.txt", header=T)
data$Node = factor(data$Node)
data$Kilobits <- data$Kilobytes * 8
data$Type = factor(data$Type)

## data.rtr = data[grep("Rtr", data$Node),]

# combine stats from all faces
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)

data.prod = subset (data.combined, Node %in% c("p1", "p2", "p3", "p4"))

# graph rates on the root nodes in Packets
g.root <- ggplot(data.prod) +
  geom_point(aes (x=Time, y=Packets.sum, color=Type), size=1) +
  geom_line(aes (x=Time, y=Packets.sum, color=Type), size=0.5) +
  ylab("Rate [Kbits/s]") +
  facet_wrap(~ Node)

print(g.root)
