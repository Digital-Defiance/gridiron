build:
	mkdir -p /workspaces/gridiron/build
	cd /workspaces/gridiron/build && cmake /workspaces/gridiron && make

clean:
	rm -rf /workspaces/gridiron/build