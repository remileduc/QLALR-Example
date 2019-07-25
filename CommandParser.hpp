
#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include <QList>
#include <QRegularExpression>

#include "Command.hpp"
#include "QParser.hpp"
#include "parser_p.h"

class CommandParser : public QParser<CommandParser, Parser, QStringRef>
{
public:
	virtual ~CommandParser() override = default;

	void init() { command.reset(); }
	int nextToken();
	void consumeRule(int ruleno);

	void setInput(QString input);
	const Command &getCommand() { return command; }

private:
	QString inputs;
	QList<QStringRef> tokens;
	QStringRef yylval;
	Command command;
};

#endif // COMMANDPARSER_HPP
