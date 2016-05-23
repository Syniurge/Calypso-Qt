 /*
    Contributed by Elie 'Syniurge' Morisse (syniurge@gmail.com)

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <QtGlobal>
#include <QString>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <clang-c/Index.h>

namespace {
    QString includeFolder;

    QTextStream& qStdOut()
    {
        static QTextStream ts(stdout);
        return ts;
    }

    class CXStringEx : public CXString {
    public:
        CXStringEx () {}
        CXStringEx (const CXString s) { data = s.data; private_flags = s.private_flags; }
        ~CXStringEx () { clang_disposeString(*this); }

        CXStringEx& operator= (const CXString& s) { data = s.data; private_flags = s.private_flags; return *this; }
        operator CXString() { return (*static_cast<CXString *> (this)); }
    };

    QString GetCursorFilenameLocation (CXCursor cursor)
    {
        CXFile cursorFile;
        clang_getSpellingLocation (clang_getCursorLocation (cursor), &cursorFile, nullptr, nullptr, nullptr);

        CXStringEx cursorFilename = clang_getFileName (cursorFile);
        QDir dir( QString(clang_getCString(cursorFilename)) );

        return dir.absolutePath();
    }

    struct EnumContext
    {
        QString recordName;
        QString enumName;
    };

    CXChildVisitResult CursorVisitor_Enum (CXCursor cursor, CXCursor, CXClientData client_data)
    {
        auto enumCtx = static_cast<EnumContext*>(client_data);

        if ( clang_getCursorKind (cursor) == CXCursor_EnumConstantDecl )
        {
            CXStringEx cursorSpelling = clang_getCursorSpelling(cursor);
            QString constantName = clang_getCString(cursorSpelling);

            if (!enumCtx->recordName.isEmpty()) {
                qStdOut() << "QT_ENUM_CONSTANT(" << enumCtx->recordName << "::" << constantName
                                    << ", " << enumCtx->recordName << "." << enumCtx->enumName << "." << constantName << ")\n";
            } else {
                qStdOut() << "QT_ENUM_CONSTANT(" << constantName
                                    << ", " << enumCtx->enumName << "." << constantName << ")\n";
            }
        }

        return CXChildVisit_Continue;
    }

    CXChildVisitResult CursorVisitor (CXCursor cursor, CXCursor parent, CXClientData client_data)
    {
        auto cursorFilename = GetCursorFilenameLocation (cursor);

        // We first check if the declaration is located inside the Qt folder
        if (!cursorFilename.startsWith(includeFolder))
            return CXChildVisit_Continue;

        auto AS = clang_getCXXAccessSpecifier(cursor);
        if ( AS == CX_CXXPrivate || AS == CX_CXXProtected )
            return CXChildVisit_Continue;

        // Checks if this isn't an à-posteriori definition
        if ( !clang_equalCursors(clang_getCursorLexicalParent(cursor), clang_getCursorSemanticParent(cursor)) )
            return CXChildVisit_Continue;

        // Checks if this isn't just a preliminary declaration
        CXCursor cursorDefinition = clang_getCursorDefinition(cursor);
        if (!clang_Cursor_isNull(cursorDefinition) && !clang_isCursorDefinition(cursor)
                && clang_equalCursors(clang_getCursorLexicalParent(cursorDefinition), clang_getCursorSemanticParent(cursorDefinition))) // the last condition checks whether the definition occurs later but without leaving the same scope declaration
            return CXChildVisit_Continue;

        switch (clang_getCursorKind (cursor))
        {
            case CXCursor_Namespace:
            case CXCursor_StructDecl:
            case CXCursor_ClassDecl:
                clang_visitChildren (cursor, &CursorVisitor, client_data);
                break;

            case CXCursor_EnumDecl:
                {
                    EnumContext enumCtx;

                    CXStringEx cursorSpelling = clang_getCursorSpelling(cursor);
                    enumCtx.enumName = clang_getCString(cursorSpelling);

                    if (enumCtx.enumName.isEmpty())
                        break;

                    if (clang_getCursorKind(parent) == CXCursor_StructDecl
                                    || clang_getCursorKind(parent) == CXCursor_ClassDecl)
                    {
                        CXStringEx parentSpelling = clang_getCursorSpelling(parent);
                        enumCtx.recordName = clang_getCString(parentSpelling);
                    }

                    clang_visitChildren (cursor, &CursorVisitor_Enum, &enumCtx);
                }
                break;

            default:
                break;
        }

        return CXChildVisit_Continue;
    }
}

int main (int argc, const char **argv)
{
    if (argc < 2) {
        qStdOut() << "Usage: enumgen <Qt include folder>\n";
        exit(1);
    }

    QDir includeDir = QString(argv[1]);
    includeFolder = includeDir.absolutePath();

    QString includeParam = "-I";
    includeParam += includeFolder;
    auto ba_includeParam = includeParam.toUtf8();
    auto c_includeParam = ba_includeParam.data();

    CXTranslationUnit transUnit;
    auto idx = clang_createIndex(0, 0);
    if (auto errCode = clang_parseTranslationUnit2 (idx, "uic_widgets.hpp", &c_includeParam, 1,
                                            nullptr, 0, CXTranslationUnit_Incomplete, &transUnit)) {
        exit((int) errCode);
    }

    for (unsigned i = 0, n = clang_getNumDiagnostics(transUnit); i != n; ++i) {
        auto diag = clang_getDiagnostic(transUnit, i);
        auto str = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());

        qCritical() << clang_getCString(str);
        clang_disposeString(str);
    }

    unsigned result = clang_visitChildren (clang_getTranslationUnitCursor(transUnit),
                                           &CursorVisitor, nullptr);

    clang_disposeTranslationUnit(transUnit);
    clang_disposeIndex(idx);

    return result;
}
