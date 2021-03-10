#include "AST.h"
//ASTFunctionLoad
void ASTFunctionLoad::HandleTranslationUnit(ASTContext &Context){
	TranslationUnitDecl *D = Context.getTranslationUnitDecl();
	TraverseDecl(D);
}

bool ASTFunctionLoad::VisitFunctionDecl(FunctionDecl *FD) {
	if (FD && FD->isThisDeclarationADefinition()) {
		
		if (dyn_cast<CXXMethodDecl>(FD) != nullptr)
		{
			functions.push_back(FD);
		}
			// Add C non-inline function 
		 if (!FD->isInlined()){
			functions.push_back(FD);
		}
	}
	return true;
}

const std::vector<FunctionDecl *> &ASTFunctionLoad::getFunctions() const
{
	return functions;
}

//ASTGlobalVarDeclLoad
void ASTGlobalVarDeclLoad::HandleTranslationUnit(ASTContext &Context)
{
	TranslationUnitDecl *D = Context.getTranslationUnitDecl();
	TraverseDecl(D);
}

bool ASTGlobalVarDeclLoad::VisitGlobalVarDecl(VarDecl *VD)
{
	globalVars.push_back(VD);
	return true;
}

const std::vector<VarDecl *>& ASTGlobalVarDeclLoad::getGlobalVarDecls() const
{
	return globalVars;
}


//ASTCalledFunctionLoad
bool ASTCalledFunctionLoad::VisitCallExpr(CallExpr *E) {
	if (FunctionDecl *FD = E->getDirectCallee()) {
		functions.push_back(FD);
	}
	return true;
}

const std::vector<FunctionDecl *>& ASTCalledFunctionLoad::getFunctions() {
	return functions;
}

//ASTCallExprLoad
bool ASTCallExprLoad::VisitCallExpr(CallExpr *E) {
	call_exprs.push_back(E);
	return true;
}

const std::vector<CallExpr *>& ASTCallExprLoad::getCallExprs() {
	return call_exprs;
}

//ASTVarDeclLoad
bool ASTVarDeclLoad::VisitDeclStmt(DeclStmt *S) {
	for (auto D : S->decls()) {
		if (VarDecl *VD = dyn_cast<VarDecl>(D)) {
			variables.push_back(VD);
		}
	}
	return true;
}
const std::vector<VarDecl *>& ASTVarDeclLoad::getVariables() {
	return variables;
}

//ASTCXXRecordLoad
void ASTCXXRecordLoad::HandleTranslationUnit(ASTContext &Context){
	TranslationUnitDecl *D = Context.getTranslationUnitDecl();
	TraverseDecl(D);
}

bool ASTCXXRecordLoad::VisitCXXRecordDecl(CXXRecordDecl *rd) {
	if (rd && rd->isThisDeclarationADefinition())
	{
		cxxrds.push_back(rd);
	}
	return true;
}

const std::vector<CXXRecordDecl *>& ASTCXXRecordLoad::getClassDecl() {
	return cxxrds;
}

//ASTCXXMethodDecl
bool ASTCXXMethodDeclLoad::VisitCXXMethodDecl(CXXMethodDecl *cxxmd) {
	if (cxxmd && cxxmd->isThisDeclarationADefinition())
	{
		cxxmds.push_back(cxxmd);
	}
	return true;
}

const std::vector<CXXMethodDecl *>& ASTCXXMethodDeclLoad::getCXXMethodDecl() {
	return cxxmds;
}

//ASTFieldDeclLoad
bool ASTFieldDeclLoad::VisitFieldDecl(FieldDecl *fd)
{
	fds.push_back(fd);
	return true;
}
const std::vector<FieldDecl *>& ASTFieldDeclLoad::getFieldDecl()
{
	return fds;
}
