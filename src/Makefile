#
#	Copyright (c) 2014 Colorado State University
#
#     Permission is hereby granted, free of charge, to any person
#     obtaining a copy of this software and associated documentation
#     files (the "Software"), to deal in the Software without
#     restriction, including without limitation the rights to use,
#     copy, modify, merge, publish, distribute, sublicense, and/or
#     sell copies of the Software, and to permit persons to whom
#     the Software is furnished to do so, subject to the following
#     conditions:
#
#     The above copyright notice and this permission notice shall be
#     included in all copies or substantial portions of the Software.
#
#     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#     HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#     WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#     OTHER DEALINGS IN THE SOFTWARE.
#
#

CXX = g++
PROG = mksim
CXXFLAGS = -g -O2 -Wfatal-errors -Wshadow -Wall -std=c++11 -lstdc++ -L./HElib-2015-06-17/src/fhe.a -L./libtomcrypt/libtomcrypt.a -I./HElib-2015-06-17/src -I./ntl-9.2.0/include -I./libtomcrypt/src/headers -L/usr/local/lib -I/usr/include/boost
LDFLAGS = -lboost_system -lboost_filesystem -lboost_serialization -pthread -lgmp -lm ./HElib-2015-06-17/src/fhe.a ./libtomcrypt/libtomcrypt.a ./ntl-9.2.0/src/ntl.a

OBJECTDIR = ../obj
BINDIR = ../bin

all: $(PROG)

clean:
	rm -f $(PROG) $(OBJECTS)

OBJECTS = $(OBJECTDIR)/Config.o \
          $(OBJECTDIR)/DecryptSimilarityMeasureTask.o \
          $(OBJECTDIR)/Dictionary.o \
	  $(OBJECTDIR)/DocumentIndex.o \
          $(OBJECTDIR)/DocumentSimilarity.o \
          $(OBJECTDIR)/EncryptTFIndexTask.o \
	  $(OBJECTDIR)/Event.o \
          $(OBJECTDIR)/EventHandler.o \
          $(OBJECTDIR)/EventQueue.o \
	  $(OBJECTDIR)/FileManager.o \
	  $(OBJECTDIR)/HEManager.o \
	  $(OBJECTDIR)/IndexBuildEngine.o \
	  $(OBJECTDIR)/Log.o \
	  $(OBJECTDIR)/LTCManager.o \
	  $(OBJECTDIR)/Mutex.o \
	  $(OBJECTDIR)/MutexHandler.o \
	  $(OBJECTDIR)/MutexRW.o \
	  $(OBJECTDIR)/Queue.o \
          $(OBJECTDIR)/ReadContentEvent.o \
	  $(OBJECTDIR)/ReadContentTask.o \
	  $(OBJECTDIR)/SearchEngine.o \
	  $(OBJECTDIR)/SearchQuery.o \
	  $(OBJECTDIR)/SimilarityMeasureEvent.o \
	  $(OBJECTDIR)/SimilarityMeasureResults.o \
	  $(OBJECTDIR)/SimilarityMeasureTask.o \
	  $(OBJECTDIR)/SimSearch.o \
	  $(OBJECTDIR)/SSE2Index.o \
	  $(OBJECTDIR)/SSE2SearchQuery.o \
	  $(OBJECTDIR)/SSE2SearchResults.o \
	  $(OBJECTDIR)/StatsTracker.o \
	  $(OBJECTDIR)/Task.o \
	  $(OBJECTDIR)/TF.o \
	  $(OBJECTDIR)/TFIndex.o \
	  $(OBJECTDIR)/TFIndexTask.o \
	  $(OBJECTDIR)/Thread.o \
	  $(OBJECTDIR)/ThreadPool.o

$(PROG): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINDIR)/$(PROG)

$(OBJECTDIR)/Config.o: Config.cpp
	$(CXX) $(CXXFLAGS) -c Config.cpp -o $(OBJECTDIR)/Config.o

$(OBJECTDIR)/DecryptSimilarityMeasureTask.o: DecryptSimilarityMeasureTask.cpp
	$(CXX) $(CXXFLAGS) -c DecryptSimilarityMeasureTask.cpp -o $(OBJECTDIR)/DecryptSimilarityMeasureTask.o

$(OBJECTDIR)/Dictionary.o: Dictionary.cpp
	$(CXX) $(CXXFLAGS) -c Dictionary.cpp -o $(OBJECTDIR)/Dictionary.o

$(OBJECTDIR)/DocumentIndex.o: DocumentIndex.cpp
	$(CXX) $(CXXFLAGS) -c DocumentIndex.cpp -o $(OBJECTDIR)/DocumentIndex.o

$(OBJECTDIR)/DocumentSimilarity.o: DocumentSimilarity.cpp
	$(CXX) $(CXXFLAGS) -c DocumentSimilarity.cpp -o $(OBJECTDIR)/DocumentSimilarity.o

$(OBJECTDIR)/EncryptTFIndexTask.o: EncryptTFIndexTask.cpp
	$(CXX) $(CXXFLAGS) -c EncryptTFIndexTask.cpp -o $(OBJECTDIR)/EncryptTFIndexTask.o

$(OBJECTDIR)/Event.o: Event.cpp
	$(CXX) $(CXXFLAGS) -c Event.cpp -o $(OBJECTDIR)/Event.o

$(OBJECTDIR)/EventHandler.o: EventHandler.cpp
	$(CXX) $(CXXFLAGS) -c EventHandler.cpp -o $(OBJECTDIR)/EventHandler.o

$(OBJECTDIR)/EventQueue.o: EventQueue.cpp
	$(CXX) $(CXXFLAGS) -c EventQueue.cpp -o $(OBJECTDIR)/EventQueue.o

$(OBJECTDIR)/FileManager.o: FileManager.cpp
	$(CXX) $(CXXFLAGS) -c FileManager.cpp -o $(OBJECTDIR)/FileManager.o

$(OBJECTDIR)/HEManager.o: HEManager.cpp
	$(CXX) $(CXXFLAGS) -c HEManager.cpp -o $(OBJECTDIR)/HEManager.o

$(OBJECTDIR)/IndexBuildEngine.o: IndexBuildEngine.cpp
	$(CXX) $(CXXFLAGS) -c IndexBuildEngine.cpp -o $(OBJECTDIR)/IndexBuildEngine.o

$(OBJECTDIR)/Log.o: Log.cpp
	$(CXX) $(CXXFLAGS) -c Log.cpp -o $(OBJECTDIR)/Log.o

$(OBJECTDIR)/LTCManager.o: LTCManager.cpp
	$(CXX) $(CXXFLAGS) -c LTCManager.cpp -o $(OBJECTDIR)/LTCManager.o

$(OBJECTDIR)/Mutex.o: Mutex.cpp
	$(CXX) $(CXXFLAGS) -c Mutex.cpp -o $(OBJECTDIR)/Mutex.o

$(OBJECTDIR)/MutexHandler.o: MutexHandler.cpp
	$(CXX) $(CXXFLAGS) -c MutexHandler.cpp -o $(OBJECTDIR)/MutexHandler.o

$(OBJECTDIR)/MutexRW.o: MutexRW.cpp
	$(CXX) $(CXXFLAGS) -c MutexRW.cpp -o $(OBJECTDIR)/MutexRW.o

$(OBJECTDIR)/Queue.o: Queue.cpp
	$(CXX) $(CXXFLAGS) -c Queue.cpp -o $(OBJECTDIR)/Queue.o

$(OBJECTDIR)/SearchEngine.o: SearchEngine.cpp
	$(CXX) $(CXXFLAGS) -c SearchEngine.cpp -o $(OBJECTDIR)/SearchEngine.o

$(OBJECTDIR)/SearchQuery.o: SearchQuery.cpp
	$(CXX) $(CXXFLAGS) -c SearchQuery.cpp -o $(OBJECTDIR)/SearchQuery.o

$(OBJECTDIR)/SimilarityMeasureEvent.o: SimilarityMeasureEvent.cpp
	$(CXX) $(CXXFLAGS) -c SimilarityMeasureEvent.cpp -o $(OBJECTDIR)/SimilarityMeasureEvent.o

$(OBJECTDIR)/SimilarityMeasureResults.o: SimilarityMeasureResults.cpp
	$(CXX) $(CXXFLAGS) -c SimilarityMeasureResults.cpp -o $(OBJECTDIR)/SimilarityMeasureResults.o

$(OBJECTDIR)/SimilarityMeasureTask.o: SimilarityMeasureTask.cpp
	$(CXX) $(CXXFLAGS) -c SimilarityMeasureTask.cpp -o $(OBJECTDIR)/SimilarityMeasureTask.o

$(OBJECTDIR)/SimSearch.o: SimSearch.cpp
	$(CXX) $(CXXFLAGS) -c SimSearch.cpp -o $(OBJECTDIR)/SimSearch.o

$(OBJECTDIR)/SSE2Index.o: SSE2Index.cpp
	$(CXX) $(CXXFLAGS) -c SSE2Index.cpp -o $(OBJECTDIR)/SSE2Index.o

$(OBJECTDIR)/SSE2SearchQuery.o: SSE2SearchQuery.cpp
	$(CXX) $(CXXFLAGS) -c SSE2SearchQuery.cpp -o $(OBJECTDIR)/SSE2SearchQuery.o

$(OBJECTDIR)/SSE2SearchResults.o: SSE2SearchResults.cpp
	$(CXX) $(CXXFLAGS) -c SSE2SearchResults.cpp -o $(OBJECTDIR)/SSE2SearchResults.o

$(OBJECTDIR)/StatsTracker.o: StatsTracker.cpp
	$(CXX) $(CXXFLAGS) -c StatsTracker.cpp -o $(OBJECTDIR)/StatsTracker.o

$(OBJECTDIR)/Task.o: Task.cpp
	$(CXX) $(CXXFLAGS) -c Task.cpp -o $(OBJECTDIR)/Task.o

$(OBJECTDIR)/ReadContentEvent.o: ReadContentEvent.cpp
	$(CXX) $(CXXFLAGS) -c ReadContentEvent.cpp -o $(OBJECTDIR)/ReadContentEvent.o

$(OBJECTDIR)/ReadContentTask.o: ReadContentTask.cpp
	$(CXX) $(CXXFLAGS) -c ReadContentTask.cpp -o $(OBJECTDIR)/ReadContentTask.o
	
$(OBJECTDIR)/TF.o: TF.cpp
	$(CXX) $(CXXFLAGS) -c TF.cpp -o $(OBJECTDIR)/TF.o

$(OBJECTDIR)/TFIndex.o: TFIndex.cpp
	$(CXX) $(CXXFLAGS) -c TFIndex.cpp -o $(OBJECTDIR)/TFIndex.o

$(OBJECTDIR)/TFIndexTask.o: TFIndexTask.cpp
	$(CXX) $(CXXFLAGS) -c TFIndexTask.cpp -o $(OBJECTDIR)/TFIndexTask.o

$(OBJECTDIR)/Thread.o: Thread.cpp
	$(CXX) $(CXXFLAGS) -c Thread.cpp -o $(OBJECTDIR)/Thread.o

$(OBJECTDIR)/ThreadPool.o: ThreadPool.cpp
	$(CXX) $(CXXFLAGS) -c ThreadPool.cpp -o $(OBJECTDIR)/ThreadPool.o
