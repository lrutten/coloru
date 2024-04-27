
(defn tel 
   [[ a & [b & c] ]] (println a "---" b "---" c))

(tel '(11 22 33 44))

