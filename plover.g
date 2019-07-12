----------------------------------------------------------------------------
--
-- Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
-- Contact: Nokia Corporation (qt-info@nokia.com)
-- Copyright (C) 2019 Rémi Ducceschi
--
-- This file is part of the documentation of Qt. It was originally
-- published as part of Qt Quarterly.
--
-- $QT_BEGIN_LICENSE:LGPL$
-- Commercial Usage
-- Licensees holding valid Qt Commercial licenses may use this file in
-- accordance with the Qt Commercial License Agreement provided with the
-- Software or, alternatively, in accordance with the terms contained in
-- a written agreement between you and Nokia.
--
-- GNU Lesser General Public License Usage
-- Alternatively, this file may be used under the terms of the GNU Lesser
-- General Public License version 2.1 as published by the Free Software
-- Foundation and appearing in the file LICENSE.LGPL included in the
-- packaging of this file.  Please review the following information to
-- ensure the GNU Lesser General Public License version 2.1 requirements
-- will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
--
-- In addition, as a special exception, Nokia gives you certain additional
-- rights.  These rights are described in the Nokia Qt LGPL Exception
-- version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
--
-- GNU General Public License Usage
-- Alternatively, this file may be used under the terms of the GNU
-- General Public License version 3.0 as published by the Free Software
-- Foundation and appearing in the file LICENSE.GPL included in the
-- packaging of this file.  Please review the following information to
-- ensure the GNU General Public License version 3.0 requirements will be
-- met: http://www.gnu.org/copyleft/gpl.html.
--
-- If you have questions regarding the use of this file, please contact
-- Nokia at qt-info@nokia.com.
-- $QT_END_LICENSE$
--
----------------------------------------------------------------------------

%parser Parser
%decl CommandParser.hpp
%impl CommandParser.cpp

-- The verbs
%token EAT "eat"
%token EXAMINE "examine"
%token GO "go"
%token NORTH "north"
%token EAST "east"
%token SOUTH "south"
%token WEST "west"
%token PUT "put"

-- The preposition
%token IN "in"

-- Object names
%token THE "the"
%token OBJECTNAME "object"

%start Input

/:
#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include <QList>
#include <QRegularExpression>

#include "Command.hpp"
#include "QParser.hpp"
#include "$header"

class CommandParser : public QParser<CommandParser, $table, QString>
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
:/

/.
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
./

Input: Sentence;

Sentence: GO SingleVerbSentence
	| SingleVerbSentence
	| OneNounSentence
	| TwoNounSentence;

ObjectName: OBJECTNAME;

Noun: THE ObjectList
	| ObjectList;

ObjectList: ObjectList ObjectName;
/.
	case $rule_number:
		command.nounNames.append(sym(2));
		break;
./
ObjectList: ObjectName;
/.
	case $rule_number:
		command.nounNames.append(sym(1));
		break;
./

Second: THE SecondList
	| SecondList;

SecondList: ObjectName;
/.
	case $rule_number:
		command.secondNames.append(sym(1));
		break;
./
SecondList: SecondList ObjectName;
/.
	case $rule_number:
		command.secondNames.append(sym(2));
		break;
./

SingleVerbSentence: NORTH;
/.
	case $rule_number:
		command.verb = Command::North;
		break;
./
SingleVerbSentence: EAST;
/.
	case $rule_number:
		command.verb = Command::East;
		break;
./
SingleVerbSentence: SOUTH;
/.
	case $rule_number:
		command.verb = Command::South;
		break;
./
SingleVerbSentence: WEST;
/.
	case $rule_number:
		command.verb = Command::West;
		break;
./

OneNounSentence: OneNounVerb Noun;

OneNounVerb: EAT;
/.
	case $rule_number:
		command.verb = Command::Eat;
		break;
./
OneNounVerb: EXAMINE;
/.
	case $rule_number:
		command.verb = Command::Examine;
		break;
./

TwoNounSentence: PUT Noun IN Second;
/.
	case $rule_number:
		command.verb = Command::Put;
		command.preposition = Command::In;
		break;
./

/.
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
./
