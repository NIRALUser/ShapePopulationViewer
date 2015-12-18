#include "testChangeView.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testChangeView(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->CreateNewWindow(filename);

    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);

    std::vector< double* > view;
    double P[6] = {0,-1,0,0,0,1}; // View P
    view.push_back(P);
    double A[6] = {0,1,0,0,0,1}; // View A
    view.push_back(A);
    double L[6] = {-1,0,0,0,0,1}; // View L
    view.push_back(L);
    double R[6] = {1,0,0,0,0,1}; // View R
    view.push_back(R);
    double S[6] = {0,0,1,0,1,0}; // View S
    view.push_back(S);
    double I[6] = {0,0,-1,0,1,0}; // View I
    view.push_back(I);


    for(int i = 0; i < 6; i++)
    {
        // Call of the function that must be test
        shapePopulationBase->ChangeView(view[i][0],view[i][1],view[i][2],view[i][3],view[i][4],view[i][5]);

        // Test if the result obtained is correct
        vtkSmartPointer<vtkRenderer> firstRenderer = vtkSmartPointer<vtkRenderer>::New();
        firstRenderer = shapePopulationBase->m_windowsList[0]->GetRenderers()->GetFirstRenderer();
        double* coords = firstRenderer->GetActiveCamera()->GetFocalPoint();
        double distance = firstRenderer->GetActiveCamera()->GetDistance();
        double* viewUp = firstRenderer->GetActiveCamera()->GetViewUp();
        double* position = firstRenderer->GetActiveCamera()->GetPosition();

        if(!(position[0] == (coords[0] + view[i][0]*distance) && position[1] == (coords[1] + view[i][1]*distance) && position[2] == (coords[2] + view[i][2]*distance) && viewUp[0] == view[i][3] && viewUp[1] == view[i][4] && viewUp[2] == view[i][5]))
        {
            return 1;
        }
    }
    return 0;
}
