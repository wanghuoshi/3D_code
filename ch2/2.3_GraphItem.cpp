#include "vtkCommand.h"
#include "vtkRandomGraphSource.h"
#include "vtkGraph.h"
#include "vtkGraphItem.h"
#include "vtkVariant.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkObjectFactory.h"
#include "vtkRegressionTestImage.h"

class GraphAnimate : public vtkCommand
{
public:
    static GraphAnimate *New()
    {
        return new GraphAnimate();
    }

    vtkTypeMacro(GraphAnimate, vtkCommand);

    virtual void Execute(vtkObject *, unsigned long, void *)
    {
        this->GraphItem->UpdateLayer();
        this->View->Render();
        this->View->GetRenderWindow()->GetInteractor()->CreateOneShotTimer(10);
    }

private:
    vtkGraphItem *GraphItem;
    vtkContextView *View;
};

int main(int, char *[])
{
    vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderer()->SetBackground(1.0,1.0,1.0);
    view->GetRenderWindow()->SetSize(800,600);
    
    vtkSmartPointer<vtkRandomGraphSource> source = vtkSmartPointer<vtkRandomGraphSource>::New();
    source->SetNumberOfVertices(100);
    source->SetNumberOfEdges(0);
    source->StartWithTreeOn();
    source->Update();
    
    vtkSmartPointer<vtkGraphItem> item = vtkSmartPointer<vtkGraphItem>::New();
    item->SetGraph(source->GetOutput());
    view->GetScene()->AddItem(item);

    vtkSmartPointer<GraphAnimate> anim = vtkSmartPointer<GraphAnimate>::New();
    anim->View = view;
    anim->GraphItem = item;
    view->GetRenderWindow()->GetInteractor()->Initialize();
    view->GetRenderWindow()->GetInteractor()->CreateOneShotTimer();
    view->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::TimerEvent,anim);
    view->GetRenderWindow()->GetInteractor()->Start();
}