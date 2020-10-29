(defn lijst [] '(11 22 33 44 55))

(defn tel 
   [[ a b & [c & d] ]] (println a "---" b "---" c "---" d))

(tel (lijst))

