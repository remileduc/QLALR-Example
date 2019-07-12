/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
** Copyright (C) 2019 Rémi Ducceschi
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <array>

#include <QDebug>
#include <QString>

#include "CommandParser.hpp"

// Testing the parser
int main(int argv, char **args)
{
	std::array<QString, 11> sentences = {
		"north", // OK
		"go north", // OK
		"eat the very good and tasty apple", // OK
		"put orange peel in the trash bin", // OK
		"go go west", // NOK
		"west west", // NOK
		"put peel in in the trash bin", // NOK
		"west go", // NOK
		"eat the hot dog in the park", // NOK
		"examine pizza", // OK
		"eat" // NOK
	};

	CommandParser interpreter;
	for (const auto &sentence : sentences)
	{
		interpreter.setInput(sentence);
		if (interpreter.parse())
		{
			qDebug() << sentence << ": OK";
			qDebug() << "\tnoun:" << interpreter.getCommand().nounNames << "- second:" << interpreter.getCommand().secondNames;
		}
		else
		{
			qDebug() << sentence << ": NOK";
			qDebug() << "\terror:" << interpreter.error();
		}
		qDebug();
	}
}
