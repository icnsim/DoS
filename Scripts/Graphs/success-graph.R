# drop-graph.R
# Gera gráficos de pacotes recebidos.

# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)

#########################
# Rate trace processing #
#########################
data = read.table("/home/aluno/ndnSIM/ns-3/rate-trace.txt", header=T)
data$Node = factor(data$Node)
data$FaceId <- factor(data$FaceId)
data$Packets <- data$Packets
data$Type = factor(data$Type)

# exlude irrelevant types
data = subset(data, Type %in% c("SatisfiedInterests"))

# combine stats from all faces
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)

data.prod = subset (data.combined, Node %in% c("p1", "p2", "p3", "p4"))
data.cli = subset(data.combined, Node %in% c("c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9"))
data.cli2 = subset(data.combined, Node %in% c("c10", "c11", "c12", "c13", "c14", "c15", "c16", "c17", "c18"))
data.rout = subset(data.combined, Node %in% c("n1", "n2", "n3", "n4", "n5", "n6", "n7", "n8", "n9"))

# graph rates on all clients in Kilobits
g.cli <- ggplot(data.cli) +
  geom_point(aes (x=Time, y=Packets.sum, color=Type), size=1) +
  ylab("Rate [Kbits/s]") +
  facet_wrap(~ Node)

print(g.cli)

# graph rates on all clients in Kilobits
g.cli2 <- ggplot(data.cli2) +
  geom_point(aes (x=Time, y=Packets.sum, color=Type), size=1) +
  ylab("Rate [Kbits/s]") +
  facet_wrap(~ Node)

print(g.cli2)

# graph rates on all routers in Kilobits
g.rout <- ggplot(data.rout) +
  geom_point(aes (x=Time, y=Packets.sum, color=Type), size=1) +
  ylab("Rate [Kbits/s]") +
  facet_wrap(~ Node)

print(g.rout)

# graph rates on the root nodes in Packets
g.root <- ggplot(data.prod) +
  geom_point(aes (x=Time, y=Packets.sum, color=Type), size=1) +
  geom_line(aes (x=Time, y=Packets.sum, color=Type), size=0.5) +
  ylab("Rate [Kbits/s]") +
  facet_wrap(~ Node)

print(g.root)

png("graphs/root-rates.png", width=500, height=250)
print(g.root)
retval <- dev.off()
