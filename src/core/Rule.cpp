#include "Rule.h"
#include "Piece.h"
#include "Board.h"

bool Rule::canConnect(const Piece* a, const Piece* b) {
      if (a == nullptr || b == nullptr) {
            return false;
      }
      if (a->getPieceOwner() != b->getPieceOwner()) {
            return false;
      }
      return true;
    
}

bool Rule::canPlacePiece(const Piece* piecePos) {
      if (piecePos != nullptr) {
            return false;
      }
      return true;
}

bool Rule::canbeSelect(const Piece* PieceID, PlayerID ID) {
      if (PieceID->getPieceOwner() != ID) {
            return false;
      }
      return true;
}

bool Rule::canGrow(const Board* board, const int fromRow, const int fromCol, const int toRow, const int toCol, const PlayerID player) {
      
      

      // 检查是否有棋子并且属于自己
      const Piece* fromPiece = board->getPieceAt(fromRow, fromCol);
      std::cout << "fromRow fromCol = " << fromRow << " " << fromCol << "\n";
      std::cout << "toRow toCol = " << toRow << " " << toCol << "\n";
      if (fromPiece == nullptr || fromPiece->getPieceOwner() != player) {
            std::cout << "not belong to player\n";
            return false;
      }
      
      
      if (board->getPieceAt(toRow, toCol) != nullptr) {
            return false; // 有棋子不能生长
      }
      
      int fromPieceID = board->getPieceID(fromRow, fromCol);
      int toPieceID = board->getPieceID(toRow, toCol);
      //std::cout << "fromPieceID= " << fromPieceID << ", toPieceID= " << toPieceID << "\n";
      auto selectedComponent = board->getComponentByPieceID(fromPieceID);
      std::cout << "before action Component size: " << selectedComponent.size() << "\n";
      for (auto PieceID : selectedComponent) {
            auto Neighbors = board->getOrthogonalNeighbors(PieceID);
            if (Neighbors.find(toPieceID) != Neighbors.end()) {
                  return true;
            }
      }
      return false;
}

bool Rule::canMove(const Board* board, const int fromRow, const int fromCol, const int toRow, const int toCol, const PlayerID player) {
      
      // 检查是否有棋子并且属于自己
      const Piece* fromPiece = board->getPieceAt(fromRow, fromCol);
      std::cout << "fromRow fromCol = " << fromRow << " " << fromCol << "\n";
      std::cout << "toRow toCol = " << toRow << " " << toCol << "\n";
      if (fromPiece == nullptr || fromPiece->getPieceOwner() != player) {
            return false;
      }
      // 检查是否是对方棋子或者空格
      const Piece* toPiece = board->getPieceAt(toRow, toCol);
      if (toPiece != nullptr && toPiece->getPieceOwner() == player) {
            return false;
      }

      int fromPieceID = board->getPieceID(fromRow, fromCol);
      int toPieceID = board->getPieceID(toRow, toCol);
      auto selectedComponent = board->getComponentByPieceID(fromPieceID);
      std::cout << "before action Component size: " << selectedComponent.size() << "\n";
      for (auto PieceID : selectedComponent) {
            auto Neighbors = board->getOrthogonalNeighbors(PieceID);
            if (Neighbors.find(toPieceID) != Neighbors.end()) {
                  return true;
            }
      }
      return false;
}

bool Rule::canSpore(const Board* board, const int fromRow, const int fromCol, const int toRow, const int toCol, const PlayerID player) {
      // 检查是否有棋子并且属于自己
      const Piece* fromPiece = board->getPieceAt(fromRow, fromCol);
      std::cout << "fromRow fromCol = " << fromRow << " " << fromCol << "\n";
      std::cout << "toRow toCol = " << toRow << " " << toCol << "\n";
      if (fromPiece == nullptr || fromPiece->getPieceOwner() != player) {
            return false;
      }
      
      
      if (board->getPieceAt(toRow, toCol) != nullptr) {
            return false; // 有棋子不能扩散
      }

      int fromPieceID = board->getPieceID(fromRow, fromCol);
      int toPieceID = board->getPieceID(toRow, toCol);
      auto selectedComponent = board->getComponentByPieceID(fromPieceID);
      std::cout << "before action Component size: " << selectedComponent.size() << "\n";
      for (auto PieceID : selectedComponent) {
            auto SporeRegion = board->getSporeRange(PieceID);
            if (SporeRegion.find(toPieceID) != SporeRegion.end()) {
                  return true;
            }
      }
      return false;

}