#include "Wikiformat.h"



void Wikiformat::wikicode()
	{
	std::wifstream inputFile("C:/Users/satis/OneDrive/Personal_Projects/WIP/CPP/MBh01.txt");
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	inputFile.imbue(loc);
	std::wstring line;
	std::multimap<std::wstring, std::wstring> verseIdsToSlokaMultimap;
	std::multimap<std::wstring, std::wstring> chapterIdToVerseIdsMultiMap;
	std::set<std::wstring> chapters;

	int lineno = 0;
	if (inputFile.is_open())
	{
		while (std::getline(inputFile, line)) {
			if (line.substr(0, 1).c_str()[0] == '%')
				continue;
			size_t pos = line.find(' ', 0);
			if (pos == std::wstring::npos)
			{
				std::cout << "no key in lineno" << lineno << std::endl;
			}
			std::wstring keystr = line.substr(0, pos);
			if (isalpha(keystr.back()))
				keystr.pop_back();
			std::wstring valstr = line.substr(pos + 1, std::wstring::npos);
			verseIdsToSlokaMultimap.insert(std::pair<std::wstring, std::wstring>(keystr, valstr));

			std::wstring chapterId = keystr.substr(2, 3);
			chapterIdToVerseIdsMultiMap.insert(std::pair<std::wstring, std::wstring>(chapterId, keystr));

			if (chapters.find(chapterId) == chapters.end())
				chapters.insert(chapterId);

		}
	}
	inputFile.close();

	std::set<std::wstring> verseKeys;         //set to store the unique keys

	wchar_t singlePipe = L'\u0964';
	wchar_t doublePipe = L'\u0965';

	for (auto chapterIterator = chapters.begin(); chapterIterator != chapters.end(); chapterIterator++)
	{
		std::wstring chapterId = *chapterIterator;
		auto chapterIdToVerseIdsMultiMapIterator = chapterIdToVerseIdsMultiMap.equal_range(chapterId);
		std::wofstream formattedFileA(L"C:/Users/satis/OneDrive/Personal_Projects/WIP/CPP/output/MBh01-" + chapterId + L".txt", std::ios::out | std::ios::ate);
		std::locale locO(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
		formattedFileA.imbue(locO);

		std::set<std::wstring> alreadyProcessedVerseIds;
		for (auto chapterIdToVerseIdsMultiMapIteratorPair = chapterIdToVerseIdsMultiMapIterator.first; chapterIdToVerseIdsMultiMapIteratorPair != chapterIdToVerseIdsMultiMapIterator.second; ++chapterIdToVerseIdsMultiMapIteratorPair) {

			std::wstring currentVerseId = chapterIdToVerseIdsMultiMapIteratorPair->second;
			if (checkIfCurrentVerseIdIsAlreadyProcessed(alreadyProcessedVerseIds, currentVerseId))
			{
				alreadyProcessedVerseIds.insert(currentVerseId);
				formattedFileA << "|-" << std::endl << "| " << chapterIdToVerseIdsMultiMapIteratorPair->second << " || ";
				auto its = verseIdsToSlokaMultimap.equal_range(chapterIdToVerseIdsMultiMapIteratorPair->second);
				std::wstring verseStr;
				for (auto itr = its.first; itr != its.second; itr++) {
					verseStr += itr->second + singlePipe + L" <br>";
				}
				formattedFileA << verseStr.substr(0, verseStr.length() - 6);
				formattedFileA << L"   " << doublePipe << (chapterIdToVerseIdsMultiMapIteratorPair->second) << doublePipe << L"||";
				formattedFileA << std::endl;
			}

		}
		formattedFileA.close();
	}
	// std::cout << " count of keys " << verseKeys.size()  << " count of lines" << verseIdsToSlokaMultimap.size() << std::endl;
}
