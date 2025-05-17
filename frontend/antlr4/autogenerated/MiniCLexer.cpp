
// Generated from MiniC.g4 by ANTLR 4.12.0


#include "MiniCLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct MiniCLexerStaticData final {
  MiniCLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  MiniCLexerStaticData(const MiniCLexerStaticData&) = delete;
  MiniCLexerStaticData(MiniCLexerStaticData&&) = delete;
  MiniCLexerStaticData& operator=(const MiniCLexerStaticData&) = delete;
  MiniCLexerStaticData& operator=(MiniCLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag miniclexerLexerOnceFlag;
MiniCLexerStaticData *miniclexerLexerStaticData = nullptr;

void miniclexerLexerInitialize() {
  assert(miniclexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<MiniCLexerStaticData>(
    std::vector<std::string>{
      "T_L_PAREN", "T_R_PAREN", "T_SEMICOLON", "T_L_BRACE", "T_R_BRACE", 
      "T_ASSIGN", "T_COMMA", "T_ADD", "T_SUB", "T_MUL", "T_DIV", "T_MOD", 
      "T_LT", "T_GT", "T_LE", "T_GE", "T_EQ", "T_NEQ", "T_AND", "T_OR", 
      "T_NOT", "T_RETURN", "T_INT", "T_VOID", "T_IF", "T_ELSE", "T_WHILE", 
      "T_BREAK", "T_CONTINUE", "T_ID", "T_DIGIT", "WS", "COMMENT"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'('", "')'", "';'", "'{'", "'}'", "'='", "','", "'+'", "'-'", 
      "'*'", "'/'", "'%'", "'<'", "'>'", "'<='", "'>='", "'=='", "'!='", 
      "'&&'", "'||'", "'!'", "'return'", "'int'", "'void'", "'if'", "'else'", 
      "'while'", "'break'", "'continue'"
    },
    std::vector<std::string>{
      "", "T_L_PAREN", "T_R_PAREN", "T_SEMICOLON", "T_L_BRACE", "T_R_BRACE", 
      "T_ASSIGN", "T_COMMA", "T_ADD", "T_SUB", "T_MUL", "T_DIV", "T_MOD", 
      "T_LT", "T_GT", "T_LE", "T_GE", "T_EQ", "T_NEQ", "T_AND", "T_OR", 
      "T_NOT", "T_RETURN", "T_INT", "T_VOID", "T_IF", "T_ELSE", "T_WHILE", 
      "T_BREAK", "T_CONTINUE", "T_ID", "T_DIGIT", "WS", "COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,33,212,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,1,0,1,0,1,1,1,1,1,2,1,2,
  	1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,
  	1,11,1,12,1,12,1,13,1,13,1,14,1,14,1,14,1,15,1,15,1,15,1,16,1,16,1,16,
  	1,17,1,17,1,17,1,18,1,18,1,18,1,19,1,19,1,19,1,20,1,20,1,21,1,21,1,21,
  	1,21,1,21,1,21,1,21,1,22,1,22,1,22,1,22,1,23,1,23,1,23,1,23,1,23,1,24,
  	1,24,1,24,1,25,1,25,1,25,1,25,1,25,1,26,1,26,1,26,1,26,1,26,1,26,1,27,
  	1,27,1,27,1,27,1,27,1,27,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,
  	1,29,1,29,5,29,163,8,29,10,29,12,29,166,9,29,1,30,1,30,1,30,4,30,171,
  	8,30,11,30,12,30,172,1,30,1,30,1,30,1,30,3,30,179,8,30,1,30,4,30,182,
  	8,30,11,30,12,30,183,1,30,1,30,5,30,188,8,30,10,30,12,30,191,9,30,3,30,
  	193,8,30,1,31,4,31,196,8,31,11,31,12,31,197,1,31,1,31,1,32,1,32,1,32,
  	1,32,5,32,206,8,32,10,32,12,32,209,9,32,1,32,1,32,0,0,33,1,1,3,2,5,3,
  	7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,
  	33,17,35,18,37,19,39,20,41,21,43,22,45,23,47,24,49,25,51,26,53,27,55,
  	28,57,29,59,30,61,31,63,32,65,33,1,0,8,3,0,65,90,95,95,97,122,4,0,48,
  	57,65,90,95,95,97,122,1,0,48,55,3,0,48,57,65,70,97,102,1,0,49,57,1,0,
  	48,57,3,0,9,10,13,13,32,32,2,0,10,10,13,13,221,0,1,1,0,0,0,0,3,1,0,0,
  	0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,
  	1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,
  	0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,
  	0,37,1,0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,0,43,1,0,0,0,0,45,1,0,0,0,0,47,
  	1,0,0,0,0,49,1,0,0,0,0,51,1,0,0,0,0,53,1,0,0,0,0,55,1,0,0,0,0,57,1,0,
  	0,0,0,59,1,0,0,0,0,61,1,0,0,0,0,63,1,0,0,0,0,65,1,0,0,0,1,67,1,0,0,0,
  	3,69,1,0,0,0,5,71,1,0,0,0,7,73,1,0,0,0,9,75,1,0,0,0,11,77,1,0,0,0,13,
  	79,1,0,0,0,15,81,1,0,0,0,17,83,1,0,0,0,19,85,1,0,0,0,21,87,1,0,0,0,23,
  	89,1,0,0,0,25,91,1,0,0,0,27,93,1,0,0,0,29,95,1,0,0,0,31,98,1,0,0,0,33,
  	101,1,0,0,0,35,104,1,0,0,0,37,107,1,0,0,0,39,110,1,0,0,0,41,113,1,0,0,
  	0,43,115,1,0,0,0,45,122,1,0,0,0,47,126,1,0,0,0,49,131,1,0,0,0,51,134,
  	1,0,0,0,53,139,1,0,0,0,55,145,1,0,0,0,57,151,1,0,0,0,59,160,1,0,0,0,61,
  	192,1,0,0,0,63,195,1,0,0,0,65,201,1,0,0,0,67,68,5,40,0,0,68,2,1,0,0,0,
  	69,70,5,41,0,0,70,4,1,0,0,0,71,72,5,59,0,0,72,6,1,0,0,0,73,74,5,123,0,
  	0,74,8,1,0,0,0,75,76,5,125,0,0,76,10,1,0,0,0,77,78,5,61,0,0,78,12,1,0,
  	0,0,79,80,5,44,0,0,80,14,1,0,0,0,81,82,5,43,0,0,82,16,1,0,0,0,83,84,5,
  	45,0,0,84,18,1,0,0,0,85,86,5,42,0,0,86,20,1,0,0,0,87,88,5,47,0,0,88,22,
  	1,0,0,0,89,90,5,37,0,0,90,24,1,0,0,0,91,92,5,60,0,0,92,26,1,0,0,0,93,
  	94,5,62,0,0,94,28,1,0,0,0,95,96,5,60,0,0,96,97,5,61,0,0,97,30,1,0,0,0,
  	98,99,5,62,0,0,99,100,5,61,0,0,100,32,1,0,0,0,101,102,5,61,0,0,102,103,
  	5,61,0,0,103,34,1,0,0,0,104,105,5,33,0,0,105,106,5,61,0,0,106,36,1,0,
  	0,0,107,108,5,38,0,0,108,109,5,38,0,0,109,38,1,0,0,0,110,111,5,124,0,
  	0,111,112,5,124,0,0,112,40,1,0,0,0,113,114,5,33,0,0,114,42,1,0,0,0,115,
  	116,5,114,0,0,116,117,5,101,0,0,117,118,5,116,0,0,118,119,5,117,0,0,119,
  	120,5,114,0,0,120,121,5,110,0,0,121,44,1,0,0,0,122,123,5,105,0,0,123,
  	124,5,110,0,0,124,125,5,116,0,0,125,46,1,0,0,0,126,127,5,118,0,0,127,
  	128,5,111,0,0,128,129,5,105,0,0,129,130,5,100,0,0,130,48,1,0,0,0,131,
  	132,5,105,0,0,132,133,5,102,0,0,133,50,1,0,0,0,134,135,5,101,0,0,135,
  	136,5,108,0,0,136,137,5,115,0,0,137,138,5,101,0,0,138,52,1,0,0,0,139,
  	140,5,119,0,0,140,141,5,104,0,0,141,142,5,105,0,0,142,143,5,108,0,0,143,
  	144,5,101,0,0,144,54,1,0,0,0,145,146,5,98,0,0,146,147,5,114,0,0,147,148,
  	5,101,0,0,148,149,5,97,0,0,149,150,5,107,0,0,150,56,1,0,0,0,151,152,5,
  	99,0,0,152,153,5,111,0,0,153,154,5,110,0,0,154,155,5,116,0,0,155,156,
  	5,105,0,0,156,157,5,110,0,0,157,158,5,117,0,0,158,159,5,101,0,0,159,58,
  	1,0,0,0,160,164,7,0,0,0,161,163,7,1,0,0,162,161,1,0,0,0,163,166,1,0,0,
  	0,164,162,1,0,0,0,164,165,1,0,0,0,165,60,1,0,0,0,166,164,1,0,0,0,167,
  	193,5,48,0,0,168,170,5,48,0,0,169,171,7,2,0,0,170,169,1,0,0,0,171,172,
  	1,0,0,0,172,170,1,0,0,0,172,173,1,0,0,0,173,193,1,0,0,0,174,175,5,48,
  	0,0,175,179,5,120,0,0,176,177,5,48,0,0,177,179,5,88,0,0,178,174,1,0,0,
  	0,178,176,1,0,0,0,179,181,1,0,0,0,180,182,7,3,0,0,181,180,1,0,0,0,182,
  	183,1,0,0,0,183,181,1,0,0,0,183,184,1,0,0,0,184,193,1,0,0,0,185,189,7,
  	4,0,0,186,188,7,5,0,0,187,186,1,0,0,0,188,191,1,0,0,0,189,187,1,0,0,0,
  	189,190,1,0,0,0,190,193,1,0,0,0,191,189,1,0,0,0,192,167,1,0,0,0,192,168,
  	1,0,0,0,192,178,1,0,0,0,192,185,1,0,0,0,193,62,1,0,0,0,194,196,7,6,0,
  	0,195,194,1,0,0,0,196,197,1,0,0,0,197,195,1,0,0,0,197,198,1,0,0,0,198,
  	199,1,0,0,0,199,200,6,31,0,0,200,64,1,0,0,0,201,202,5,47,0,0,202,203,
  	5,47,0,0,203,207,1,0,0,0,204,206,8,7,0,0,205,204,1,0,0,0,206,209,1,0,
  	0,0,207,205,1,0,0,0,207,208,1,0,0,0,208,210,1,0,0,0,209,207,1,0,0,0,210,
  	211,6,32,0,0,211,66,1,0,0,0,9,0,164,172,178,183,189,192,197,207,1,6,0,
  	0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  miniclexerLexerStaticData = staticData.release();
}

}

MiniCLexer::MiniCLexer(CharStream *input) : Lexer(input) {
  MiniCLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *miniclexerLexerStaticData->atn, miniclexerLexerStaticData->decisionToDFA, miniclexerLexerStaticData->sharedContextCache);
}

MiniCLexer::~MiniCLexer() {
  delete _interpreter;
}

std::string MiniCLexer::getGrammarFileName() const {
  return "MiniC.g4";
}

const std::vector<std::string>& MiniCLexer::getRuleNames() const {
  return miniclexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& MiniCLexer::getChannelNames() const {
  return miniclexerLexerStaticData->channelNames;
}

const std::vector<std::string>& MiniCLexer::getModeNames() const {
  return miniclexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& MiniCLexer::getVocabulary() const {
  return miniclexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView MiniCLexer::getSerializedATN() const {
  return miniclexerLexerStaticData->serializedATN;
}

const atn::ATN& MiniCLexer::getATN() const {
  return *miniclexerLexerStaticData->atn;
}




void MiniCLexer::initialize() {
  ::antlr4::internal::call_once(miniclexerLexerOnceFlag, miniclexerLexerInitialize);
}
