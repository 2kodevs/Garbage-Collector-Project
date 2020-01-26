name = "new_garbage"
garbage_dir = "garbage_files/"
v = untitlle
t = garbageTable.txt
g = garbage.txt
move_func = move.so
f = ./$(move_func)
editor = "subl"

edit: ## Open the makefile in editor
	$(editor) Makefile

so%: fdm%.c ## Create the movement shared library
	gcc -shared -fPIC $^ -o $(move_func)

build: ## Compile the app binary
	gcc main.c -ldl -o bin.out

run: ## Run the app
	./bin.out $(t) $(f)

new: ## Generate a new garbage file
	./generator.out $(g)

save: ## Save the current garbage file
	cp garbage.txt $(garbage_dir)$(name).txt

help: ## Show this help
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
