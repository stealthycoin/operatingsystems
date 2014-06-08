TO INSTALL OUR CHANGES:

- start with a fresh minix 3.1.8
- upload src/ and tst/ to said minix install (make sure they are in the same directory)
- enter tst/ and run tst/do.sh (this will put the files form src/ (provided it is at the same level as tst/) into their proper places under /usr/src
- then go to /usr/src/tools and run: make clean install
- restart minix using the 2 option


TO TEST OUR INSTALLED CHANGES:

- enter the tst/ directory and run: make
- to metatag a file: ./metatag FILENAME "TAG"
- to metacat a file: ./metacat FILENAME


TO DEMONSTRATE...:
...adding a note “This file is awesome!” to a README.txt file, and later reading back the note.
echo "test" > README.txt
./metatag README.txt "This file is awesome!"
./metacat README.txt

...that changing the regular file contents does not change the extra metadata.
echo "test" > R
./metatag R "meta"
echo "test2" >> R
./metacat R

...that changing the metadata does not change the regular file contents.
echo "test" > R
./metatag R "meta"
cat R

...that copying a file with metadata copies the metadata.
echo "test" > R
./metatag R "meta"
cp R R2
./metacat R2

...that changing the metadata on the original file does not modify the metadata of the copied file.
echo "test" > R
./metatag R "meta"
cp R R2
./metatag R2 "meta2"
./metacat R

...that creating 1000 files, adding metadata to them, then deleting them does not decrease the free space on the filesystem.
df -h
for f in {1..1000}; do echo "$f" > "$f".txt; ./metatag "$f".txt "meta"; done
rm *.txt
df -h
