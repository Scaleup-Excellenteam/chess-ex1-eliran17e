// Chess
#include "../include/Chess.h"
#include "../include/Board.h"
#include "MyExceptions.h"

int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
//	string board = "##########K###############################R#############r#rk####";
    Board b(board);
	Chess a(board);
    int depth;
    int codeResponse = 0;
    std::cout << "Enter AI depth: ";
    if (!(std::cin >> depth)) {
        std::cerr << "Invalid input for depth.\n";
        return 1;
    }
    try{

        auto suggestions = b.getTopMoves(3,depth);
        std::cout << "Top 3 suggested moves:\n";
        for (const auto& move : suggestions) {
            std::cout << move<< "\n";
        }
    }catch (const MyExceptions& e){
        std::cerr << "\033[1;31m" << e.what() << "\033[0m\n";
    }


    string res = a.getInput();
	while (res != "exit")
	{
		/*
		codeResponse value :
		Illegal movements :
		11 - there is not piece at the source
		12 - the piece in the source is piece of your opponent
		13 - there one of your pieces at the destination
		21 - illegal movement of that piece
		31 - this movement will cause you checkmate

		legal movements :
		41 - the last movement was legal and cause check
		42 - the last movement was legal, next turn
		*/

		/**/
		{ // put your code here instead that code
            codeResponse = b.checkMove(res);
            if (codeResponse == 41 || codeResponse == 42) {
                try{
                    auto suggestions = b.getTopMoves(3,depth);
                    std::cout << "Top 3 suggested moves:\n";
                    for (const auto& move : suggestions) {
                        std::cout << move<< "\n";
                    }
                }catch (const MyExceptions& e){
                    std::cerr << "\033[1;31m" << e.what() << "\033[0m\n";
                }
            }

		}
		/**/

		a.setCodeResponse(codeResponse);
		res = a.getInput();
	}

	cout << endl << "Exiting " << endl;
	return 0;
}



