import os, sys, csv

if len(sys.argv) != 2 :
    print("\nManque d'un argument.\n")
else :
    fr = open(sys.argv[1], "r");
    name = sys.argv[1][:len(sys.argv[1]) - 4]
    with open(name + ".csv", "w", newline='') as csvfile :
        for line in fr.readlines() :
            li = line.split("|");
            li = [e.replace(',', '') for e in li]
            if (li[-1] == "\n") :
                li.pop(-1)
            spamwriter = csv.writer(csvfile, delimiter=",", quoting=csv.QUOTE_ALL)
            spamwriter.writerow(li)
