(defn lijst [] '(11 22 33 44))

(defn tel 
   [[c d & [e & f]] g] (println c "---" d "---" e "---" f "---" g))

(tel '(1 2 3 4 5) 6)
'(tel (lijst))
