/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
** Copyright (C) 2019 Rémi Ducceschi
**
** This file is part of the QLALR module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QPARSER_HPP
#define QPARSER_HPP

#include <vector>

#include <QStringList>

template<class _Parser, class _Table, class _Value = int>
class QParser : protected _Table
{
public:
	QParser() : _tos(0) {}
	virtual ~QParser() = default;
	QParser(const QParser &) = delete;
	QParser(const QParser &&) = delete;
	QParser &operator=(const QParser &) = delete;
	QParser &operator=(const QParser &&) = delete;

	bool parse();
	inline _Value &sym(int index);
	const QString &error() const noexcept { return _error; }

private:
	inline void init()
	{
		_tos = 0;
		_stack.clear();
		_stack.resize(128);
		_error.clear();
		static_cast<_Parser *>(this)->init();
	}
	inline int nextToken() { return static_cast<_Parser *>(this)->nextToken(); }
	inline void consumeRule(int rule) { static_cast<_Parser *>(this)->consumeRule(rule); }

private:
	struct _ParseStack
	{
		int state = 0;
		_Value parsed;
	};
	std::vector<_ParseStack> _stack;
	size_t _tos;
	QString _error;
};

template<class _Parser, class _Table, class _Value>
inline _Value &QParser<_Parser, _Table, _Value>::sym(int n)
{
	return _stack[_tos + n - 1].parsed;
}

template<class _Parser, class _Table, class _Value>
bool QParser<_Parser, _Table, _Value>::parse()
{
	init();

	const int INITIAL_STATE = 0;
	int act = _stack[++_tos].state = INITIAL_STATE;
	int token = -1;

	while (true)
	{
		if (token == -1 && (-_Table::TERMINAL_COUNT != _Table::action_index[act]))
			token = nextToken();

		act = _Table::t_action(act, token);

		// end of rule
		if (_stack[_tos].state == _Table::ACCEPT_STATE)
			return true;
		// need next token
		else if (act > 0)
		{
			if (++_tos == _stack.size())
				_stack.resize(_tos * 2);
			_stack[_tos].parsed = _stack[_tos - 1].parsed;
			_stack[_tos].state = act;
			token = -1;
		}
		// production complete, need to parse
		else if (act < 0)
		{
			int r = -act - 1;
			_tos -= _Table::rhs[r];
			act = _stack[_tos++].state;
			consumeRule(r);
			act = _stack[_tos].state = _Table::nt_action(act, _Table::lhs[r] - _Table::TERMINAL_COUNT);
		}
		// error handling
		else
		{
			if (token == _Table::EOF_SYMBOL)
				_error = "Unexpected end of document.";
			else
			{
				QStringList expected;
				const int ers = _stack[_tos].state;

				for (int tk = 0; tk < _Table::TERMINAL_COUNT; tk++)
				{
					if ((_Table::t_action(ers, tk) > 0) && _Table::spell[tk])
						expected.push_back(_Table::spell[tk]);
				}

				// if possible, we list all the expected symbols instead of the found one
				if (!expected.isEmpty())
				{
					// last expected symbols
					QString exp_str = expected.takeLast();
					// if more than 1 symbol
					if (!expected.isEmpty())
						exp_str = expected.join("', '") + "', or '" + exp_str;
					_error = QString("Expected '%1', but got '%2'.").arg(exp_str, _Table::spell[token]);
				}
				// otherwise, we just put the unexpected symbol
				else
					_error = QString("Unexpected '%1'.").arg(_Table::spell[token]);
			}
			break;
		}
	}

	return false;
}

#endif // QPARSER_HPP
