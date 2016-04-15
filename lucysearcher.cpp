#include "lucysearcher.h"

lucysearcher::lucysearcher(logger* pLogger, QObject *parent) : lucy(pLogger, parent), m_pIndexSearcher(nullptr)
{

}

/*
IndexReader* reader = IndexReader::open("../Index");

lucene::analysis::SimpleAnalyzer* analyzer =
    new lucene::analysis::SimpleAnalyzer();

IndexReader* newreader = reader->reopen();
if ( newreader != reader )
{
    _CLLDELETE(reader);
    reader = newreader;
}
IndexSearcher searcher(reader);


Query* query = QueryParser::parse(L"Nixon*",
    L"president", analyzer);
Hits* hits = searcher.search(query);
cout << "Total hits: " << hits->length() << endl;
*/


/*
wstring search_string = L"Where is it?";

lucene::index::IndexReader    *index_reader;
lucene::search::IndexSearcher *index_searcher;
Query                         *index_query;
Hits                          *index_hits;
WhitespaceAnalyzer            analyser;

index_reader   = IndexReader::open( args_info.index_arg );
index_searcher = new IndexSearcher(index_reader);
index_query    = QueryParser::parse( search_string.c_str(), L"content", &analyser );
index_hits     = index_searcher->search(index_query);
if ( index_hits->length() > 0 ) {
    for( long i=0; i < index_hits->length(); i++ ) {
        Document &doc = index_hits->doc(i);
        wcout << "FOUND: " << doc.get(L"filename") << endl;
    }
}

delete index_hits;
delete index_query;

index_reader->close();
delete index_searcher;
*/
