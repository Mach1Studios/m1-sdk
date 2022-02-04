### Install Dependencies:

 - Install libsndfile and dependencies via this tutorial: [Link](http://www.linuxfromscratch.org/blfs/view/svn/multimedia/libsndfile.html)
 	Breakdown of tutorial above:
	 - `wget https://downloads.xiph.org/releases/ogg/libogg-1.3.3.tar.xz && tar xf libogg-1.3.3.tar.xz`
	 - `cd libogg-1.3.3 && ./configure --prefix=/usr --docdir=/usr/share/doc/libogg-1.3.3 && make`
	 - `make install`
	 - `cd ../`
	 - `wget https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.6.tar.xz && tar xf libvorbis-1.3.6.tar.xz`
	 - `cd libvorbis.1.3.6 && ./configure --prefix/usr && make`
	 - `make install && install -v -m644 doc/Vorbis* /usr/share/doc/libvorbis-1.3.6`
	 - `cd ../`
	 - `wget https://downloads.xiph.org/releases/flac/flac-1.3.2.tar.xz && tar xf flac-1.3.2.tar.xz`
	 - `cd flac-1.3.2 && ./configure --prefix=/usr --disable-thorough-tests && make`
	 - `make install`
	 - `cd ../`
	 - `wget http://www.mega-nerd.com/libsndfile/files/libsndfile-1.0.28.tar.gz && tar xf libsndfile-1.0.28.tar.gz`
	 - `cd libsndfile-1.0.28 && ./configure --prefix=/usr --docdir=/usr/share/doc/libsndfile-1.0.28 && make`
	 - `make install`
	 - `cd ../` 
