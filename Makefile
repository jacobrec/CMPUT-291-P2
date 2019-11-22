tar:
	tar --exclude='./prj2.tgz' --exclude='./.git' --exclude='./testing' -czvaf prj2.tgz .

prepare:
	mkdir -p output
	bash -c ". ./projectUtils.sh; makeCLibs"

.PHONY: tar prepare
