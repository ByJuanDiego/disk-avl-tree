//
// Created by juandiego on 4/28/23.
//

#include "../inc/avl.hpp"
#include "../inc/record.hpp"

int main() {
    std::string heap_file = "./database/movies_and_series.dat";
    std::string index_file = "./database/avl_indexed_by_certificate.dat";

    std::function<char *(MovieRecord &)> index = [](MovieRecord &movie) { return movie.certificate; };
    std::function<bool(char[16], char[16])> greater = [](char a[16], char b[16]) -> bool {
        return std::string(a) > std::string(b);
    };

    AVLFile<char[16], MovieRecord, decltype(index), decltype(greater)> avl(heap_file, index_file, false, index,
                                                                           greater);

    char to_search[16];
    std::cout << "Enter the certificate: ";
    func::read_buffer(to_search, 16);
    for (MovieRecord& record : avl.search(to_search, heap_file)) {
        std::cout << record.to_string() << std::endl;
    }

//    char to_remove[16];
//    std::cout << "Enter the certificate to remove: ";
//    func::read_buffer(to_remove, 16);
//    avl.remove(to_remove, heap_file);
//    avl.queued_report();
}

/*
Initial state of the tree...

[0]: <key: R, pointer: 10764, height: 5, left: 560, right: 280, next: 5689880>
[560]: <key: M, pointer: 2274516, height: 4, left: 3192, right: 224, next: 5477808>
[280]: <key: TV-Y7, pointer: 1172448, height: 4, left: 112, right: 6440, next: 5687976>
[3192]: <key: Approved, pointer: 47196, height: 3, left: 27944, right: 19208, next: 5668544>
[224]: <key: PG, pointer: 4140, height: 3, left: 25928, right: 728, next: 5675544>
[112]: <key: TV-MA, pointer: 3312, height: 3, left: 2856, right: 67144, next: 5689768>
[6440]: <key: Unrated, pointer: 1656, height: 2, left: 79296, right: 56, next: 5513032>
[27944]: <key: 16+, pointer: 5459832, height: 2, left: 369264, right: 468496, next: 5604592>
[19208]: <key: EM, pointer: 18323640, height: 2, left: 2839368, right: 713272, next: -1>
[25928]: <key: NC-17, pointer: 11036412, height: 2, left: 746424, right: 300048, next: 5557888>
[728]: <key: Passed, pointer: 8280, height: 1, left: 6272, right: -1, next: 5619544>
[2856]: <key: TV-14, pointer: 42228, height: 2, left: 2085104, right: 14504, next: 5689488>
[67144]: <key: TV-PG, pointer: 95220, height: 1, left: -1, right: 227024, next: 5689376>
[79296]: <key: TV-Y7-FV, pointer: 992772, height: 0, left: -1, right: -1, next: 5678792>
[56]: <key: null, pointer: 0, height: 1, left: 51688, right: -1, next: 27888>
[369264]: <key: 12, pointer: 59197032, height: 1, left: 2628752, right: 4003664, next: 4999344>
[468496]: <key: 18+, pointer: 6927048, height: 1, left: 1011192, right: 1251264, next: 4912096>
[2839368]: <key: E, pointer: 41982084, height: 1, left: -1, right: 2910432, next: 5202848>
[713272]: <key: G, pointer: 284004, height: 1, left: 1239280, right: 153832, next: 5674032>
[746424]: <key: M/PG, pointer: 4436424, height: 1, left: -1, right: 3369800, next: 5312440>
[300048]: <key: Not Rated, pointer: 828, height: 1, left: -1, right: 2736776, next: 5687528>
[6272]: <key: PG-13, pointer: 92736, height: 0, left: -1, right: -1, next: 5689824>
[2085104]: <key: TV-13, pointer: 30829752, height: 1, left: 3204040, right: -1, next: 5148752>
[14504]: <key: TV-G, pointer: 214452, height: 0, left: -1, right: -1, next: 5687080>
[227024]: <key: TV-Y, pointer: 3356712, height: 0, left: -1, right: -1, next: 5633040>
[51688]: <key: X, pointer: 764244, height: 0, left: -1, right: -1, next: 5428864>
[2628752]: <key: , pointer: 413172, height: 0, left: -1, right: -1, next: 5683048>
[4003664]: <key: 13+, pointer: 38867976, height: 0, left: -1, right: -1, next: 5642448>
[1011192]: <key: 18, pointer: 14951196, height: 0, left: -1, right: -1, next: -1>
[1251264]: <key: AO, pointer: 18500832, height: 0, left: -1, right: -1, next: 5355504>
[2910432]: <key: E10+, pointer: 43032816, height: 0, left: -1, right: -1, next: 4907280>
[1239280]: <key: F, pointer: 10546236, height: 0, left: -1, right: -1, next: 2396520>
[153832]: <key: GP, pointer: 383364, height: 0, left: -1, right: -1, next: 5524736>
[3369800]: <key: MA-17, pointer: 49824900, height: 0, left: -1, right: -1, next: 5306840>
[2736776]: <key: Open, pointer: 40465188, height: 0, left: -1, right: -1, next: 5010488>
[3204040]: <key: T, pointer: 47374020, height: 0, left: -1, right: -1, next: -1>

// After removing TV-Y (simple right rotation)

[0]: <key: R, pointer: 10764, height: 5, left: 560, right: 280, next: 5689880>
[560]: <key: M, pointer: 2274516, height: 4, left: 3192, right: 224, next: 5477808>
[280]: <key: TV-Y7, pointer: 1172448, height: 3, left: 112, right: 6440, next: 5687976>
[3192]: <key: Approved, pointer: 47196, height: 3, left: 27944, right: 19208, next: 5668544>
[224]: <key: PG, pointer: 4140, height: 3, left: 25928, right: 728, next: 5675544>
[112]: <key: TV-14, pointer: 42228, height: 2, left: 2085104, right: 2856, next: 5689488>
[6440]: <key: Unrated, pointer: 1656, height: 2, left: 79296, right: 56, next: 5513032>
[27944]: <key: 16+, pointer: 5459832, height: 2, left: 369264, right: 468496, next: 5604592>
[19208]: <key: EM, pointer: 18323640, height: 2, left: 2839368, right: 713272, next: -1>
[25928]: <key: NC-17, pointer: 11036412, height: 2, left: 746424, right: 300048, next: 5557888>
[728]: <key: Passed, pointer: 8280, height: 1, left: 6272, right: -1, next: 5619544>
[2085104]: <key: TV-13, pointer: 30829752, height: 1, left: 3204040, right: -1, next: 5148752>
[2856]: <key: TV-MA, pointer: 3312, height: 1, left: 14504, right: 67144, next: 5689768>
[79296]: <key: TV-Y7-FV, pointer: 992772, height: 0, left: -1, right: -1, next: 5678792>
[56]: <key: null, pointer: 0, height: 1, left: 51688, right: -1, next: 27888>
[369264]: <key: 12, pointer: 59197032, height: 1, left: 2628752, right: 4003664, next: 4999344>
[468496]: <key: 18+, pointer: 6927048, height: 1, left: 1011192, right: 1251264, next: 4912096>
[2839368]: <key: E, pointer: 41982084, height: 1, left: -1, right: 2910432, next: 5202848>
[713272]: <key: G, pointer: 284004, height: 1, left: 1239280, right: 153832, next: 5674032>
[746424]: <key: M/PG, pointer: 4436424, height: 1, left: -1, right: 3369800, next: 5312440>
[300048]: <key: Not Rated, pointer: 828, height: 1, left: -1, right: 2736776, next: 5687528>
[6272]: <key: PG-13, pointer: 92736, height: 0, left: -1, right: -1, next: 5689824>
[3204040]: <key: T, pointer: 47374020, height: 0, left: -1, right: -1, next: -1>
[14504]: <key: TV-G, pointer: 214452, height: 0, left: -1, right: -1, next: 5687080>
[67144]: <key: TV-PG, pointer: 95220, height: 0, left: -1, right: -1, next: 5689376>
[51688]: <key: X, pointer: 764244, height: 0, left: -1, right: -1, next: 5428864>
[2628752]: <key: , pointer: 413172, height: 0, left: -1, right: -1, next: 5683048>
[4003664]: <key: 13+, pointer: 38867976, height: 0, left: -1, right: -1, next: 5642448>
[1011192]: <key: 18, pointer: 14951196, height: 0, left: -1, right: -1, next: -1>
[1251264]: <key: AO, pointer: 18500832, height: 0, left: -1, right: -1, next: 5355504>
[2910432]: <key: E10+, pointer: 43032816, height: 0, left: -1, right: -1, next: 4907280>
[1239280]: <key: F, pointer: 10546236, height: 0, left: -1, right: -1, next: 2396520>
[153832]: <key: GP, pointer: 383364, height: 0, left: -1, right: -1, next: 5524736>
[3369800]: <key: MA-17, pointer: 49824900, height: 0, left: -1, right: -1, next: 5306840>
[2736776]: <key: Open, pointer: 40465188, height: 0, left: -1, right: -1, next: 5010488>

// After removing TV-Y7-FV (double rotation) [same as example]

      \
      (A)
    /    \
 (B)     (C)
       /
    (D)

After removing (B)

     \             \                 \
     (A)           (A)               (D)
       \             \             /    \
       (C)   ->      (D)   ->    (A)    (C)
      /                \
    (D)                (C)

first rotates to right and finally to left

[0]: <key: R, pointer: 10764, height: 5, left: 560, right: 280, next: 5689880>
[560]: <key: M, pointer: 2274516, height: 4, left: 3192, right: 224, next: 5477808>
[280]: <key: TV-Y7, pointer: 1172448, height: 3, left: 112, right: 6440, next: 5687976>
[3192]: <key: Approved, pointer: 47196, height: 3, left: 27944, right: 19208, next: 5668544>
[224]: <key: PG, pointer: 4140, height: 3, left: 25928, right: 728, next: 5675544>
[112]: <key: TV-14, pointer: 42228, height: 2, left: 2085104, right: 2856, next: 5689488>
[6440]: <key: X, pointer: 764244, height: 1, left: 56, right: 51688, next: 5428864>
[27944]: <key: 16+, pointer: 5459832, height: 2, left: 369264, right: 468496, next: 5604592>
[19208]: <key: EM, pointer: 18323640, height: 2, left: 2839368, right: 713272, next: -1>
[25928]: <key: NC-17, pointer: 11036412, height: 2, left: 746424, right: 300048, next: 5557888>
[728]: <key: Passed, pointer: 8280, height: 1, left: 6272, right: -1, next: 5619544>
[2085104]: <key: TV-13, pointer: 30829752, height: 1, left: 3204040, right: -1, next: 5148752>
[2856]: <key: TV-MA, pointer: 3312, height: 1, left: 14504, right: 67144, next: 5689768>
[56]: <key: Unrated, pointer: 1656, height: 0, left: -1, right: -1, next: 5513032>
[51688]: <key: null, pointer: 0, height: 0, left: -1, right: -1, next: 27888>
[369264]: <key: 12, pointer: 59197032, height: 1, left: 2628752, right: 4003664, next: 4999344>
[468496]: <key: 18+, pointer: 6927048, height: 1, left: 1011192, right: 1251264, next: 4912096>
[2839368]: <key: E, pointer: 41982084, height: 1, left: -1, right: 2910432, next: 5202848>
[713272]: <key: G, pointer: 284004, height: 1, left: 1239280, right: 153832, next: 5674032>
[746424]: <key: M/PG, pointer: 4436424, height: 1, left: -1, right: 3369800, next: 5312440>
[300048]: <key: Not Rated, pointer: 828, height: 1, left: -1, right: 2736776, next: 5687528>
[6272]: <key: PG-13, pointer: 92736, height: 0, left: -1, right: -1, next: 5689824>
[3204040]: <key: T, pointer: 47374020, height: 0, left: -1, right: -1, next: -1>
[14504]: <key: TV-G, pointer: 214452, height: 0, left: -1, right: -1, next: 5687080>
[67144]: <key: TV-PG, pointer: 95220, height: 0, left: -1, right: -1, next: 5689376>
[2628752]: <key: , pointer: 413172, height: 0, left: -1, right: -1, next: 5683048>
[4003664]: <key: 13+, pointer: 38867976, height: 0, left: -1, right: -1, next: 5642448>
[1011192]: <key: 18, pointer: 14951196, height: 0, left: -1, right: -1, next: -1>
[1251264]: <key: AO, pointer: 18500832, height: 0, left: -1, right: -1, next: 5355504>
[2910432]: <key: E10+, pointer: 43032816, height: 0, left: -1, right: -1, next: 4907280>
[1239280]: <key: F, pointer: 10546236, height: 0, left: -1, right: -1, next: 2396520>
[153832]: <key: GP, pointer: 383364, height: 0, left: -1, right: -1, next: 5524736>
[3369800]: <key: MA-17, pointer: 49824900, height: 0, left: -1, right: -1, next: 5306840>
[2736776]: <key: Open, pointer: 40465188, height: 0, left: -1, right: -1, next: 5010488>

// After removing the root: (key = R)

[0]: <key: T, pointer: 47374020, height: 5, left: 560, right: 280, next: -1>
[560]: <key: M, pointer: 2274516, height: 4, left: 3192, right: 224, next: 5477808>
[280]: <key: TV-Y7, pointer: 1172448, height: 3, left: 112, right: 6440, next: 5687976>
[3192]: <key: Approved, pointer: 47196, height: 3, left: 27944, right: 19208, next: 5668544>
[224]: <key: PG, pointer: 4140, height: 3, left: 25928, right: 728, next: 5675544>
[112]: <key: TV-14, pointer: 42228, height: 2, left: 2085104, right: 2856, next: 5689488>
[6440]: <key: X, pointer: 764244, height: 1, left: 56, right: 51688, next: 5428864>
[27944]: <key: 16+, pointer: 5459832, height: 2, left: 369264, right: 468496, next: 5604592>
[19208]: <key: EM, pointer: 18323640, height: 2, left: 2839368, right: 713272, next: -1>
[25928]: <key: NC-17, pointer: 11036412, height: 2, left: 746424, right: 300048, next: 5557888>
[728]: <key: Passed, pointer: 8280, height: 1, left: 6272, right: -1, next: 5619544>
[2085104]: <key: TV-13, pointer: 30829752, height: 0, left: -1, right: -1, next: 5148752>
[2856]: <key: TV-MA, pointer: 3312, height: 1, left: 14504, right: 67144, next: 5689768>
[56]: <key: Unrated, pointer: 1656, height: 0, left: -1, right: -1, next: 5513032>
[51688]: <key: null, pointer: 0, height: 0, left: -1, right: -1, next: 27888>
[369264]: <key: 12, pointer: 59197032, height: 1, left: 2628752, right: 4003664, next: 4999344>
[468496]: <key: 18+, pointer: 6927048, height: 1, left: 1011192, right: 1251264, next: 4912096>
[2839368]: <key: E, pointer: 41982084, height: 1, left: -1, right: 2910432, next: 5202848>
[713272]: <key: G, pointer: 284004, height: 1, left: 1239280, right: 153832, next: 5674032>
[746424]: <key: M/PG, pointer: 4436424, height: 1, left: -1, right: 3369800, next: 5312440>
[300048]: <key: Not Rated, pointer: 828, height: 1, left: -1, right: 2736776, next: 5687528>
[6272]: <key: PG-13, pointer: 92736, height: 0, left: -1, right: -1, next: 5689824>
[14504]: <key: TV-G, pointer: 214452, height: 0, left: -1, right: -1, next: 5687080>
[67144]: <key: TV-PG, pointer: 95220, height: 0, left: -1, right: -1, next: 5689376>
[2628752]: <key: , pointer: 413172, height: 0, left: -1, right: -1, next: 5683048>
[4003664]: <key: 13+, pointer: 38867976, height: 0, left: -1, right: -1, next: 5642448>
[1011192]: <key: 18, pointer: 14951196, height: 0, left: -1, right: -1, next: -1>
[1251264]: <key: AO, pointer: 18500832, height: 0, left: -1, right: -1, next: 5355504>
[2910432]: <key: E10+, pointer: 43032816, height: 0, left: -1, right: -1, next: 4907280>
[1239280]: <key: F, pointer: 10546236, height: 0, left: -1, right: -1, next: 2396520>
[153832]: <key: GP, pointer: 383364, height: 0, left: -1, right: -1, next: 5524736>
[3369800]: <key: MA-17, pointer: 49824900, height: 0, left: -1, right: -1, next: 5306840>
[2736776]: <key: Open, pointer: 40465188, height: 0, left: -1, right: -1, next: 5010488>

After removing (key = Passed) [just a left child]

[0]: <key: T, pointer: 47374020, height: 5, left: 560, right: 280, next: -1>
[560]: <key: M, pointer: 2274516, height: 4, left: 3192, right: 224, next: 5477808>
[280]: <key: TV-Y7, pointer: 1172448, height: 3, left: 112, right: 6440, next: 5687976>
[3192]: <key: Approved, pointer: 47196, height: 3, left: 27944, right: 19208, next: 5668544>
[224]: <key: NC-17, pointer: 11036412, height: 3, left: 746424, right: 25928, next: 5557888>
[112]: <key: TV-14, pointer: 42228, height: 2, left: 2085104, right: 2856, next: 5689488>
[6440]: <key: X, pointer: 764244, height: 1, left: 56, right: 51688, next: 5428864>
[27944]: <key: 16+, pointer: 5459832, height: 2, left: 369264, right: 468496, next: 5604592>
[19208]: <key: EM, pointer: 18323640, height: 2, left: 2839368, right: 713272, next: -1>
[746424]: <key: M/PG, pointer: 4436424, height: 1, left: -1, right: 3369800, next: 5312440>
[25928]: <key: PG, pointer: 4140, height: 2, left: 300048, right: 728, next: 5675544>
[2085104]: <key: TV-13, pointer: 30829752, height: 0, left: -1, right: -1, next: 5148752>
[2856]: <key: TV-MA, pointer: 3312, height: 1, left: 14504, right: 67144, next: 5689768>
[56]: <key: Unrated, pointer: 1656, height: 0, left: -1, right: -1, next: 5513032>
[51688]: <key: null, pointer: 0, height: 0, left: -1, right: -1, next: 27888>
[369264]: <key: 12, pointer: 59197032, height: 1, left: 2628752, right: 4003664, next: 4999344>
[468496]: <key: 18+, pointer: 6927048, height: 1, left: 1011192, right: 1251264, next: 4912096>
[2839368]: <key: E, pointer: 41982084, height: 1, left: -1, right: 2910432, next: 5202848>
[713272]: <key: G, pointer: 284004, height: 1, left: 1239280, right: 153832, next: 5674032>
[3369800]: <key: MA-17, pointer: 49824900, height: 0, left: -1, right: -1, next: 5306840>
[300048]: <key: Not Rated, pointer: 828, height: 1, left: -1, right: 2736776, next: 5687528>
[728]: <key: PG-13, pointer: 92736, height: 0, left: -1, right: -1, next: 5689824>
[14504]: <key: TV-G, pointer: 214452, height: 0, left: -1, right: -1, next: 5687080>
[67144]: <key: TV-PG, pointer: 95220, height: 0, left: -1, right: -1, next: 5689376>
[2628752]: <key: , pointer: 413172, height: 0, left: -1, right: -1, next: 5683048>
[4003664]: <key: 13+, pointer: 38867976, height: 0, left: -1, right: -1, next: 5642448>
[1011192]: <key: 18, pointer: 14951196, height: 0, left: -1, right: -1, next: -1>
[1251264]: <key: AO, pointer: 18500832, height: 0, left: -1, right: -1, next: 5355504>
[2910432]: <key: E10+, pointer: 43032816, height: 0, left: -1, right: -1, next: 4907280>
[1239280]: <key: F, pointer: 10546236, height: 0, left: -1, right: -1, next: 2396520>
[153832]: <key: GP, pointer: 383364, height: 0, left: -1, right: -1, next: 5524736>
[2736776]: <key: Open, pointer: 40465188, height: 0, left: -1, right: -1, next: 5010488>
*/
