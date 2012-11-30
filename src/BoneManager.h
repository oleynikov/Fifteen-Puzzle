#ifndef BONEMANAGER_H
#define BONEMANAGER_H

#include <QGraphicsScene>
#include <QObject>
#include <QList>
#include <Bone.h>
#include <QMessageBox>
#include <QGlobal.h>
#include <QTime>

class BoneManager : public QObject
{

    Q_OBJECT

    typedef QList<QPointF>      Cells;
    typedef Cells::iterator     CellsItr;
    typedef QList<Bone*>        Bones;
    typedef Bones::iterator     BonesItr;

    public:
                                BoneManager()
        {

            this->fieldReset();

        }
        QGraphicsScene*         getScene()
                            {

                                return &this->scene;

                            }

    private:
        static const int        shuffleMoves = 100;
        QGraphicsScene          scene;
        Bones                   bones;
        QPointF                 hole;
        bool                    shuffling;

        void                    fieldReset()
        {

            this->fieldClear();
            this->fieldFill();
            this->fieldShuffle();

        }
        void                    fieldClear()
        {

            for (BonesItr itr=this->bones.begin() ; itr!=this->bones.end() ; ++itr)
            {

                delete *itr;

            }

            this->bones.clear();

        }
        void                    fieldFill()
        {

            this->hole = QPointF(150,150);
            int boneCount = 0;

            for (int row=0 ; row<4 ; ++row)
            {

                for (int col=0 ; col<4 ; ++col)
                {

                    boneCount++;

                    if (boneCount==16)
                    {

                        break;

                    }

                    this->addBone(boneCount,QPointF(col*50,row*50));

                }

            }

        }
        void                    fieldShuffle()
        {

            QTime time = QTime::currentTime();
            qsrand((uint)time.msec());
            this->shuffling = true;

            for (int i=0 ; i<BoneManager::shuffleMoves ; ++i)
            {

                Cells cellsMoveable = this->getPointNeighbours(this->hole);
                CellsItr cellRandom = cellsMoveable.begin() + this->random(0,cellsMoveable.size()-1);
                Bone* bone = this->getBone(*cellRandom);
                this->moveBone(bone);

            }

            this->shuffling = false;

        }
        Cells                   getPointNeighbours(QPointF point)
        {

            Cells cellsAll;
            Cells cellsAvaliable;

            cellsAll.push_back(point+QPointF(0,50));
            cellsAll.push_back(point+QPointF(50,0));
            cellsAll.push_back(point+QPointF(-50,0));
            cellsAll.push_back(point+QPointF(0,-50));

            for (CellsItr itr=cellsAll.begin() ; itr!=cellsAll.end() ; ++itr)
            {

                QPointF point = *itr;

                if (point.x()>=0 && point.y()>=0 && point.x()<=150 && point.y()<=150)
                {

                    cellsAvaliable.push_back(point);

                }

            }

            return cellsAvaliable;

        }
        void                    addBone(int boneNumber, QPointF position)
        {

            QPointF point;
            Bone* bone = new Bone(boneNumber);
            this->bones.push_back(bone);
            scene.addItem(bone);
            bone->setPos(position);

            QObject::connect(bone,SIGNAL(clicked(Bone*)),this,SLOT(moveBone(Bone*)));

        }
        Bone*                   getBone(QPointF bonePosition)
        {

            Bone* bone = NULL;

            for (BonesItr itr=this->bones.begin() ; itr!=this->bones.end() ; ++itr)
            {

                if ((*itr)->pos() == bonePosition)
                {

                    bone = *itr;
                    break;

                }

            }

            return bone;

        }
        bool                    getBoneMoveable(Bone* bone)
        {

            if(this->getBoneNeighbours(bone).contains(this->hole))
            {

                return true;

            }

            return false;

        }
        Cells                   getBoneNeighbours(Bone* bone)
        {

            return this->getPointNeighbours(bone->pos());

        }
        bool                    getWinPosition()
        {

            int boneId = 0;

            for (int row=0 ; row<4 ; ++row)
            {

                for (int col=0 ; col<4 ; ++col)
                {

                    ++boneId;
                    QPointF bonePosition(col*50,row*50);
                    Bone* bone = this->getBone(bonePosition);
                    if (bone && bone->getNumber() != boneId)
                    {

                        return false;

                    }

                }

            }

            return true;

        }
        int                     random(int low, int high)
        {

            return qrand() % ((high + 1) - low) + low;

        }

    private slots:
        void                    moveBone(Bone* bone)
        {

            if(this->getBoneMoveable(bone))
            {

                QPointF bonePosOld = bone->pos();

                bone->setX(hole.x());
                bone->setY(hole.y());
                hole = bonePosOld;

                if (!this->shuffling && this->getWinPosition())
                {

                    QMessageBox msgBox;
                    msgBox.setText("You win!");
                    msgBox.exec();

                    this->fieldReset();

                }

            }

        }

};

#endif // BONEMANAGER_H
