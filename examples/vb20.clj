(defn dubbel [[g & gs]]
   (let []
      (println g " --- " gs " --- " (nil? gs))
      (if (nil? g)
         '()
         (cons 
            (* 2 g)
            (if (nil? gs)
               (let []
                  ;(println "gs nil)
                  '()
               )
               (let []
                  ;(println "gs not nil)
                  (dubbel gs)
               )
            )
         )      
      )
   )
)

(println (dubbel '(1 2 3 4 5)))

