(defn dubbel [[g & gs]]
   (let []
      (println g " --- " gs " --- " (nil? gs))
      (if (nil? g)
         '()
         (cons 
            g 
            (if (nil? gs)
               (let []
                  (println "gs nil")
                  '()
               )
               (let []
                  (println "gs not nil")
                  (dubbel gs)
               )
            )
         )      
      )
   )
)

(println (dubbel '(4 5)))

