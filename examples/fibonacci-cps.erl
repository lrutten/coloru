fibc(0, C) ->
    C(1);
fibc(1, C) ->
    C(1);
fibc(N,C) when N>1 ->
    fibc(N-1,
         fun(R1) ->
                 fibc(N-2,
                      fun(R2) ->
                              C(R1+R2)
                      end)
         end).


