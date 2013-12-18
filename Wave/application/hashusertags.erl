-module (hashusertags).
-export ([readfile/1]).
-export ([get_one_line/1]).
 
readfile (String) ->
    case file:open(String, [read]) of
        {ok, Fd} ->
            get_one_line (Fd);
        {error, Reason} ->
            io:fwrite ("file read failed"),
            {error, Reason}
    end.

get_one_line (Device) ->
    case io:get_line(Device, "") of
        eof  -> file:close(Device);
        Line -> NewLine = list_to_atom (lists:delete (10, Line)),
                io:fwrite ("~s ~w~n", [NewLine, erlang:phash2 (list_to_binary (atom_to_list (NewLine)), 2147483647)]),
                get_one_line(Device)
    end.
