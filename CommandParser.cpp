
#include <QVector>

#include "CommandParser.hpp"

void CommandParser::setInput(QString input)
{
	inputs = input;
	tokens = inputs.splitRef(QRegularExpression(R"""(\s)""")).toList();
}

void CommandParser::consumeRule(int rule)
{
	switch(rule)
	{

	case 8:
		command.nounNames.append(sym(2));
		break;

	case 9:
		command.nounNames.append(sym(1));
		break;

	case 12:
		command.secondNames.append(sym(1));
		break;

	case 13:
		command.secondNames.append(sym(2));
		break;

	case 14:
		command.verb = Command::North;
		break;

	case 15:
		command.verb = Command::East;
		break;

	case 16:
		command.verb = Command::South;
		break;

	case 17:
		command.verb = Command::West;
		break;

	case 19:
		command.verb = Command::Eat;
		break;

	case 20:
		command.verb = Command::Examine;
		break;

	case 21:
		command.verb = Command::Put;
		command.preposition = Command::In;
		break;

	} // of the switch
}

int CommandParser::nextToken()
{
	if (tokens.isEmpty())
		return EOF_SYMBOL;

	auto nextToken = tokens.takeFirst();
	sym(1) = nextToken.toString();

	if (nextToken == "eat")
		return CommandParser::EAT;
	if (nextToken == "examine")
		return CommandParser::EXAMINE;

	if (nextToken == "go")
		return CommandParser::GO;

	if (nextToken == "north")
		return CommandParser::NORTH;
	if (nextToken == "south")
		return CommandParser::SOUTH;
	if (nextToken == "west")
		return CommandParser::WEST;
	if (nextToken == "east")
		return CommandParser::EAST;

	if (nextToken == "in")
		return CommandParser::IN;

	if (nextToken == "put")
		return CommandParser::PUT;

	if (nextToken == "the")
		return CommandParser::THE;

	yylval = nextToken;
	return CommandParser::OBJECTNAME;
}
