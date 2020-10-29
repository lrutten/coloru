(defn lijst [] '(11 22 33 44))

(defn tel 
   [[c & d]] (println c "---" d))

(tel '(1 2 3))
'(tel (lijst))

