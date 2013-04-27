#include <Game.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	LD26::Game TheGame;

	if( TheGame.Initialise( ZED_TRUE ) != ZED_OK )
	{
		zedTrace( "Failed to initialise game instance\n" );
		return ZED_FAIL;
	}

	return TheGame.Execute( );
}

