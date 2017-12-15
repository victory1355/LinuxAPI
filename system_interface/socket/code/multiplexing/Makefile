all:demo_select_srv demo_select_cli

demo_select_srv:demo_select_srv.c 
	gcc demo_select_srv.c -o demo_select_srv -lvnet

demo_select_cli:demo_select_cli.c 
	gcc demo_select_cli.c -o demo_select_cli -lvnet

clean:
	rm demo_select_srv demo_select_cli
