(defn dubbel [[g & gs]]
   (println g " --- " gs " --- " (nil? gs))
)

(println (dubbel '(5)))

